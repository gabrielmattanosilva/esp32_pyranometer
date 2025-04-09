#include "wifi_module.h"
#include "sd_module.h"

const unsigned long wifiTimeout = 30000; // 30 segundos

void initWiFi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long wifiStartTime = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    if (millis() - wifiStartTime > wifiTimeout)
    {
      logSerialSD("Timeout na conexão WiFi. Continuando sem conexão...");
      break;
    }
    delay(1000);
    logSerialSD("Conectando ao WiFi...");
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    logSerialSD("Conectado ao WiFi!");
  }
}

bool isWiFiConnected()
{
  return WiFi.status() == WL_CONNECTED;
}