/**
 * @file modbus_module.h
 * @brief Interface do módulo Modbus para comunicação com o sensor PYR20
 */
#ifndef MODBUS_MODULE_H
#define MODBUS_MODULE_H

#include <stdint.h>

/**
 * @brief Inicializa a comunicação Modbus
 */
void initModbus();

/**
 * @brief Lê os dados de irradiância solar do sensor
 * @return Valor da irradiância em W/m² ou -1 em caso de erro
 */
int16_t readModbusData();

#endif