/**
 * @file adc_module.h
 * @brief Interface do módulo ADC para a leitura do BPW34
 */
#ifndef ADC_MODULE_H
#define ADC_MODULE_H

#include <stdint.h>

/**
 * @brief Inicializa o ADC
 */
void initADC();

/**
 * @brief Lê o valor do ADC e converte para irradiância em W/m²
 * @return Valor da irradiância em W/m² ou -1 em caso de erro
 */
int16_t readADC();

#endif