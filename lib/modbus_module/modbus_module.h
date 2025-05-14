/**
 * @file modbus_module.h
 * @brief Interface do módulo Modbus para comunicação com o sensor PYR20
 */
#ifndef MODBUS_MODULE_H
#define MODBUS_MODULE_H

#include <stdint.h>

void initModbus();
int16_t readModbusData();

#endif