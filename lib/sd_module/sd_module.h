#ifndef SD_MODULE_H
#define SD_MODULE_H

#include <SD.h>
#include "rtc_module.h"

extern File sdFile;

void initSD();
void logSerialSD(const char *format, ...);
bool writeDataToSD(int16_t solar_irradiance); // Mudado de void para bool
String generateLogFileName();
String generateDataFileName();

#endif