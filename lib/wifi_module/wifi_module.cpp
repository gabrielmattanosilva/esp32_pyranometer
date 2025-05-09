/**
 * @file wifi_module.cpp
 * @brief Implementação do módulo WiFi
 */

#include <WiFi.h>
#include "credentials.h"
#include "sd_module.h"
#include "wifi_module.h"

void initWiFi()
{
	const unsigned long wifiTimeout = 30000; // 30 segundos
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