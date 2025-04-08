#include <Arduino.h>
#include <RTClib.h>
#include <SD.h>
#include <ThingSpeak.h>
#include <WiFi.h>
#include "credentials.h"

RTC_DS1307 rtc;
File sdFile;
WiFiClient client;
HardwareSerial modbusSerial(2);
const uint8_t modbus_function[] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x01, 0x31, 0xCA}; // Read input registers
unsigned long previousMillis = 0;
const unsigned long logInterval = 1000;           // 1 segundo
const unsigned long SDthingSpeakInterval = 60000; // 1 minuto
const unsigned long wifiTimeout = 30000;          // 30 segundos
const int LED_PIN = 2; /* LED  ON -> Wi-Fi conectado e envio a ThingSpek com sucesso
                          LED OFF -> Wi-Fi desconectado e falha de envio ao ThingSpeak */

/* Função para gerar o nome do arquivo de log com base na data atual */
String generateLogFileName()
{
  DateTime now = rtc.now();
  char fileName[32];
  snprintf(fileName, sizeof(fileName), "/log_%04d-%02d-%02d.txt", now.year(), now.month(), now.day());
  return String(fileName);
}

/* Função para gerar o nome do arquivo de dados com base na data atual */
String generateDataFileName()
{
  DateTime now = rtc.now();
  char fileName[32];
  snprintf(fileName, sizeof(fileName), "/data_%04d-%02d-%02d.txt", now.year(), now.month(), now.day());
  return String(fileName);
}

/* Função para imprimir logs no monitor serial e salvar no cartão SD */
void logSerialSD(const char *format, ...)
{
  char logSerialSD[256];
  va_list args;
  va_start(args, format);
  vsnprintf(logSerialSD, sizeof(logSerialSD), format, args);
  va_end(args);

  // Obtém a data e hora atual
  DateTime now = rtc.now();
  unsigned long milliseconds = millis() % 1000;
  char timestamp[32];
  snprintf(timestamp, sizeof(timestamp), "%04d-%02d-%02d %02d:%02d:%02d.%03lu", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second(), milliseconds);

  // Gera o nome do arquivo de log com base na data atual
  String logFileName = generateLogFileName();

  // Abre o arquivo de log no cartão SD
  sdFile = SD.open(logFileName.c_str(), FILE_APPEND);
  if (sdFile)
  {
    sdFile.printf("[%s] %s\n", timestamp, logSerialSD);
    sdFile.close();
  }
  else
  {
    Serial.println("Erro ao abrir arquivo de log no cartão SD!");
  }

  // Exibe o log no Serial Monitor
  Serial.printf("[%s] %s\n", timestamp, logSerialSD);
}

/* Função para gravar dados de irradiância no arquivo de dados */
void writeDataToSD(int16_t solar_irradiance)
{
  DateTime now = rtc.now();
  char dataLine[64];
  snprintf(dataLine, sizeof(dataLine), "%02d-%02d-%04d %02d:%02d:%02d, %d", now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second(), solar_irradiance);

  // Gera o nome do arquivo de dados com base na data atual
  String dataFileName = generateDataFileName();

  // Abre o arquivo de dados no cartão SD
  sdFile = SD.open(dataFileName.c_str(), FILE_APPEND);
  if (sdFile)
  {
    sdFile.println(dataLine);
    sdFile.close();
    logSerialSD("Dados gravados no cartão SD com sucesso!");
  }
  else
  {
    Serial.println("Erro ao abrir arquivo de dados no cartão SD!");
  }
}

/* Função para calcular CRC-16 Modbus */
uint16_t check_crc(const uint8_t *data, uint8_t length)
{
  uint16_t crc = 0xFFFF;
  for (uint8_t i = 0; i < length; i++)
  {
    crc ^= data[i];
    for (uint8_t j = 0; j < 8; j++)
    {
      if (crc & 0x0001)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
      {
        crc >>= 1;
      }
    }
  }
  return crc;
}

/* Transmissão de dados Modbus RTU */
void modbus_tx()
{
  while (modbusSerial.available())
  {
    modbusSerial.read(); // Limpa o buffer serial
  }

  modbusSerial.write(modbus_function, sizeof(modbus_function));
  delay(10); // Pequeno atraso para garantir envio completo
}

/* Recepção de resposta Modbus RTU */
int16_t modbus_rx()
{
  uint8_t response[7];
  int i = 0;
  unsigned long startTime = millis();

  // Espera até receber dados ou atingir timeout de 100ms
  while (modbusSerial.available() == 0)
  {
    if (millis() - startTime > 100)
    {
      logSerialSD("Timeout na resposta!");
      return -1;
    }
  }

  // Recebe até 7 bytes
  while (i < 7)
  {
    if (millis() - startTime > 100)
    {
      logSerialSD("Timeout na resposta!");
      return -1;
    }
    if (modbusSerial.available())
    {
      response[i++] = modbusSerial.read();
    }
  }

  // Exibe resposta recebida
  logSerialSD("Resposta Modbus: %02X %02X %02X %02X %02X %02X %02X", response[0], response[1], response[2], response[3], response[4], response[5], response[6]);

  // Verifica CRC
  uint16_t crc_response = (response[6] << 8) | response[5];
  uint16_t crc_computed = check_crc(response, 5);
  if (crc_response != crc_computed)
  {
    logSerialSD("Erro de CRC: esperado %04X, recebido %04X", crc_computed, crc_response);
    return -1;
  }

  // Converte resposta para irradiância
  uint16_t solar_irradiance = (response[3] << 8) | response[4];
  logSerialSD("Irradiância: %d W/m^2", solar_irradiance);
  return solar_irradiance;
}

void setup()
{
  // Inicia monitor serial
  Serial.begin(115200);

  // Configura o pino do LED como saída
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Inicia com o LED apagado

  // Inicializa I2C para o RTC
  Wire.begin(21, 22); // SDA, SCL
  if (!rtc.begin())
  {
    Serial.println("Erro ao inicializar RTC!");
    while (1)
      ;
  }

  // Inicializa o módulo de cartão SD
  if (!SD.begin(5))
  {
    Serial.println("Erro ao inicializar cartão SD!");
    while (1)
      ;
  }

  // Conecta ao WiFi com timeout de 30 segundos
  WiFi.begin(SSID, PASSWORD);

  unsigned long wifiStartTime = millis();
  bool wifiConnected = false;

  while (WiFi.status() != WL_CONNECTED)
  {
    // Verifica se o tempo decorrido é maior que 30 segundos
    if (millis() - wifiStartTime > wifiTimeout)
    {
      logSerialSD("Timeout na conexão WiFi. Continuando sem conexão...");
      wifiConnected = false;
      break;
    }

    delay(1000);
    logSerialSD("Conectando ao WiFi...");
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    wifiConnected = true;
    logSerialSD("Conectado ao WiFi!");
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    logSerialSD("Não foi possível conectar ao WiFi. Continuando sem conexão...");
    digitalWrite(LED_PIN, LOW);
  }

  // Inicializa ThingSpeak (somente se conectado ao Wi-Fi)
  if (wifiConnected)
  {
    ThingSpeak.begin(client);
  }

  // Inicia leitura do PYR20
  modbusSerial.begin(9600, SERIAL_8N1, 16, 17);
  logSerialSD("Iniciando leitura do PYR20...");
}

void loop()
{
  unsigned long currentMillis = millis();

  // Verifica se passou 1 segundo
  if (currentMillis - previousMillis >= logInterval)
  {
    previousMillis = currentMillis; // Atualiza o tempo da última leitura

    // Realiza a leitura
    modbus_tx();
    int16_t solar_irradiance = modbus_rx();

    // Grava os dados no cartão SD e envia a leitura ao ThingSpeak a cada 1 minuto
    static unsigned long lastSendTime = 0;
    if (currentMillis - lastSendTime >= SDthingSpeakInterval)
    {
      lastSendTime = currentMillis;

      writeDataToSD(solar_irradiance);

      // Verifica se está conectado ao WiFi antes de enviar
      if (WiFi.status() == WL_CONNECTED)
      {
        // Envia a irradiância ao ThingSpeak
        ThingSpeak.setField(1, solar_irradiance);
        int httpCode = ThingSpeak.writeFields(CHANNEL_NUMBER, WRITE_API_KEY);

        if (httpCode == 200)
        {
          logSerialSD("Dados enviados ao ThingSpeak com sucesso!");
          digitalWrite(LED_PIN, HIGH);
        }
        else
        {
          logSerialSD("Erro ao enviar dados ao ThingSpeak. Código HTTP: %d", httpCode);
          digitalWrite(LED_PIN, LOW);
        }
      }
      else
      {
        logSerialSD("WiFi desconectado. Não foi possível enviar dados ao ThingSpeak.");
        digitalWrite(LED_PIN, LOW);
      }
    }
  }
}