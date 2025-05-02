#ifndef THINGSPEAK_MODULE_H
#define THINGSPEAK_MODULE_H

#include <stdint.h>

class WiFiClient;

void initThingSpeak();
bool sendToThingSpeak(int16_t solar_irradiance);

#endif