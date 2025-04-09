#include "thingspeak_module.h"
#include "wifi_module.h"
#include "sd_module.h"
#include <WiFiClient.h>

WiFiClient client;

void initThingSpeak() {
  if (isWiFiConnected()) {
    ThingSpeak.begin(client);
  }
}

bool sendToThingSpeak(int16_t solar_irradiance) {
  if (!isWiFiConnected()) {
    logSerialSD("WiFi desconectado. Não foi possível enviar dados ao ThingSpeak.");
    return false;
  }

  ThingSpeak.setField(1, solar_irradiance);
  int httpCode = ThingSpeak.writeFields(CHANNEL_NUMBER, WRITE_API_KEY);

  if (httpCode == 200) {
    logSerialSD("Dados enviados ao ThingSpeak com sucesso!");
    return true;
  } else {
    logSerialSD("Erro ao enviar dados ao ThingSpeak. Código HTTP: %d", httpCode);
    return false;
  }
}