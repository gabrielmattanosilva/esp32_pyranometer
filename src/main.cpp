/**
 * @file main.cpp
 * @brief Programa principal do Pyranômetro ESP32
 */

#include <Arduino.h>
#include <esp_task_wdt.h>
#include "adc_module.h"
#include "data_layer.h"
#include "modbus_module.h"
#include "pins.h"
#include "rtc_module.h"
#include "sd_module.h"
#include "thingspeak_module.h"
#include "wifi_module.h"

#define READ_PERIOD 1000  // 1 segundo -> passar para 1 minuto na versão final (6000)
#define SEND_PERIOD 60000 // 1 minuto -> passar para 10 minutos na versão final (600000)
#define BUFFER_SIZE 60    // Buffer para 60 leituras

// Buffer global para armazenamento de leituras
static CircularBuffer dataBuffer;

/**
 * @brief Função de inicialização do sistema
 */
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
    initADC();
    initThingSpeak();

    // Inicializa o buffer circular
    if (!DataLayer_init(&dataBuffer, BUFFER_SIZE))
    {
        logSerialSD("Falha ao inicializar data layer!");
        while (1)
            ; // Trava o sistema se não conseguir alocar memória
    }

    logSerialSD("Sistema inicializado.");
    logSerialSD("Memória livre inicial: %d bytes", esp_get_free_heap_size());
}
/**
 * @brief Loop principal do sistema
 * @note Executa leitura do sensor a cada 1 segundo e envio de dados a cada 1 minuto
 */
void loop()
{
    static unsigned long previousMillis = 0;
    static unsigned long lastSendTime = 0;
    unsigned long currentMillis = millis();

    yield();

    if (currentMillis - previousMillis >= READ_PERIOD)
    {
        previousMillis = currentMillis;

        // Coleta dados
        SolarReading reading;
        reading.timestamp = getCurrentTime();
        reading.irradiance_pyr20 = readModbusData();
        reading.irradiance_bpw34 = readADC();

        // Armazena na data layer
        DataLayer_addReading(&dataBuffer, &reading);

        if (currentMillis - lastSendTime >= SEND_PERIOD)
        {
            lastSendTime = currentMillis;

            // Obtém a última leitura para processamento
            SolarReading latest;
            if (DataLayer_getLatestReading(&dataBuffer, &latest))
            {
                // Gravação no SD
                if (writeDataToSD(latest.irradiance_pyr20, latest.irradiance_bpw34))
                {
                    logSerialSD("Dados gravados no SD com sucesso! [%d, %d]",
                                latest.irradiance_pyr20, latest.irradiance_bpw34);
                }

                // Envio ao ThingSpeak
                if (isWiFiConnected())
                {
                    bool success = sendToThingSpeak(latest.irradiance_pyr20, latest.irradiance_bpw34);
                    logSerialSD("Dados enviados ao ThingSpeak com sucesso! [%d, %d]",
                                latest.irradiance_pyr20, latest.irradiance_bpw34);
                    digitalWrite(LED_PIN, success ? HIGH : LOW);
                }
                else
                {
                    digitalWrite(LED_PIN, LOW);
                    logSerialSD("WiFi desconectado, pulando envio ao ThingSpeak.");
                }
            }

            logSerialSD("Memória livre: %d bytes", esp_get_free_heap_size());
        }

        esp_task_wdt_reset(); // Alimenta o watchdog
    }
}