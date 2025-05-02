#include <Arduino.h>
#include <esp_task_wdt.h>
#include "modbus_module.h"
#include "pins.h"
#include "rtc_module.h"
#include "sd_module.h"
#include "thingspeak_module.h"
#include "wifi_module.h"

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Inicializa watchdog com timeout de 30 segundos
  esp_task_wdt_init(30, true);

  initRTC();
  initSD();
  initWiFi();
  initModbus();
  initThingSpeak();

  logSerialSD("Sistema inicializado");
  logSerialSD("Memória livre inicial: %d bytes", esp_get_free_heap_size());
}

void loop()
{
  static unsigned long previousMillis = 0;
  static unsigned long lastSendTime = 0;
  unsigned long currentMillis = millis();

  yield(); // Permite que o ESP32 execute tarefas internas

  if (currentMillis - previousMillis >= 1000) // 1 segundo
  {
    previousMillis = currentMillis;

    int16_t solar_irradiance = readModbusData();

    if (currentMillis - lastSendTime >= 60000) // 1 minuto
    {
      lastSendTime = currentMillis;

      // Gravação no SD
      if (writeDataToSD(solar_irradiance))
      {
        logSerialSD("Dados gravados no SD com sucesso");
      }
      else
      {
        Serial.println("Falha ao gravar no SD");
      }

      // Envio ao ThingSpeak
      if (isWiFiConnected())
      {
        bool success = sendToThingSpeak(solar_irradiance);
        digitalWrite(LED_PIN, success ? HIGH : LOW);

        if (!success)
        {
          logSerialSD("Falha no envio ao ThingSpeak");
        }
      }
      else
      {
        digitalWrite(LED_PIN, LOW);
        logSerialSD("WiFi desconectado, pulando envio ao ThingSpeak");
      }

      logSerialSD("Memória livre: %d bytes", esp_get_free_heap_size());
    }

    esp_task_wdt_reset(); // Alimenta o watchdog
  }
}