/**
 * @file thingspeak_module.h
 * @brief Interface do módulo para envio de dados ao ThingSpeak
 */
#ifndef THINGSPEAK_MODULE_H
#define THINGSPEAK_MODULE_H

#include <stdbool.h>
#include <stdint.h>

class WiFiClient; ///< Forward declaration para WiFiClient

/**
 * @brief Inicializa o módulo ThingSpeak
 */
void initThingSpeak();

/**
 * @brief Envia dados de irradiância ao ThingSpeak
 * @param irradiance_pyr20 Valor da irradiância solar do PYR20
 * @param irradiance_bpw34 Valor da irradiância solar do BPW34
 * @return true se bem-sucedido, false caso contrário
 */
bool sendToThingSpeak(int16_t irradiance_pyr20, int16_t irradiance_bpw34);

#endif