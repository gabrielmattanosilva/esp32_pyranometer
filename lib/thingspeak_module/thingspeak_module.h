/**
 * @file thingspeak_module.h
 * @brief Interface do módulo para envio de dados ao ThingSpeak
 */
#ifndef THINGSPEAK_MODULE_H
#define THINGSPEAK_MODULE_H

#include <stdbool.h>
#include <stdint.h>

class WiFiClient; ///< Forward declaration para WiFiClient

void initThingSpeak();
bool sendToThingSpeak(int16_t irradiance_pyr20, int16_t irradiance_bpw34);

#endif