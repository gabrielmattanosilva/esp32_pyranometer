#include <Arduino.h>
#include "rtc_module.h"
#include "sd_module.h"
#include "wifi_module.h"
#include "modbus_module.h"
#include "thingspeak_module.h"

const int LED_PIN = 2; // LED indicador

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  initRTC();
  initSD();
  initWiFi();
  initModbus();

  logSerialSD("Sistema inicializado");
}

void loop()
{
  static unsigned long previousMillis = 0;
  static unsigned long lastSendTime = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 1000)
  { // 1 segundo
    previousMillis = currentMillis;

    int16_t solar_irradiance = readModbusData();

    if (currentMillis - lastSendTime >= 60000)
    { // 1 minuto
      lastSendTime = currentMillis;

      writeDataToSD(solar_irradiance);

      if (isWiFiConnected())
      {
        bool success = sendToThingSpeak(solar_irradiance);
        digitalWrite(LED_PIN, success ? HIGH : LOW);
      }
      else
      {
        digitalWrite(LED_PIN, LOW);
      }
    }
  }
}