#ifndef THINGSPEAK_MODULE_H
#define THINGSPEAK_MODULE_H

#include <ThingSpeak.h>
#include "credentials.h"

void initThingSpeak();
bool sendToThingSpeak(int16_t solar_irradiance);

#endif