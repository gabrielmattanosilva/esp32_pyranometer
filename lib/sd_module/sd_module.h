/**
 * @file sd_module.h
 * @brief Interface do módulo para armazenamento em cartão SD
 */
#ifndef SD_MODULE_H
#define SD_MODULE_H

#include <SD.h>
#include <stdbool.h>
#include <stdint.h>

extern File sdFile; ///< Arquivo atual no SD

void initSD();
void logSerialSD(const char *format, ...);
bool writeDataToSD(int16_t irradiance_pyr20, int16_t irradiance_bpw34);
String generateLogFileName();
String generateDataFileName();

#endif