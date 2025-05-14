/**
 * @file thingspeak_module.cpp
 * @brief Implementação do módulo para envio de dados ao ThingSpeak
 */

#include <ThingSpeak.h>
#include <WiFiClient.h>
#include "credentials.h"
#include "sd_module.h"
#include "thingspeak_module.h"
#include "wifi_module.h"

WiFiClient client; ///< Cliente WiFi para comunicação com o ThingSpeak

/**
 * @brief Inicializa o módulo ThingSpeak
 */
void initThingSpeak()
{
    if (isWiFiConnected())
    {
        ThingSpeak.begin(client);
    }
}

/**
 * @brief Envia dados de irradiância ao ThingSpeak
 * @param irradiance_pyr20 Valor da irradiância solar do PYR20
 * @param irradiance_bpw34 Valor da irradiância solar do BPW34
 * @return true se bem-sucedido, false caso contrário
 */
bool sendToThingSpeak(int16_t irradiance_pyr20, int16_t irradiance_bpw34)
{
    if (!isWiFiConnected())
    {
        logSerialSD("WiFi desconectado. Nao foi possivel enviar dados ao ThingSpeak.");

        return false;
    }

    ThingSpeak.setField(1, irradiance_pyr20);
    ThingSpeak.setField(2, irradiance_bpw34);
    int httpCode = ThingSpeak.writeFields(CHANNEL_NUMBER, WRITE_API_KEY);

    if (httpCode == 200)
    {
        return true;
    }
    else
    {
        logSerialSD("Erro ao enviar dados ao ThingSpeak. Codigo HTTP: %d", httpCode);

        return false;
    }
}