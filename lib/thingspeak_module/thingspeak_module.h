/**
 * @file thingspeak_module.h
 * @brief Interface do módulo para envio de dados ao ThingSpeak
 */
#ifndef THINGSPEAK_MODULE_H
#define THINGSPEAK_MODULE_H

#include <stdint.h>

class WiFiClient; ///< Forward declaration para WiFiClient

/**
 * @brief Inicializa o módulo ThingSpeak
 */
void initThingSpeak();

/**
 * @brief Envia dados de irradiância ao ThingSpeak
 * @param solar_irradiance Valor da irradiância solar
 * @return true se bem-sucedido, false caso contrário
 */
bool sendToThingSpeak(int16_t solar_irradiance);

#endif