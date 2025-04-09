#ifndef MODBUS_MODULE_H
#define MODBUS_MODULE_H

#include <HardwareSerial.h>

void initModbus();
int16_t readModbusData();

#endif