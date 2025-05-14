/**
 * @file adc_module.h
 * @brief Interface do módulo ADC para a leitura do BPW34
 */
#ifndef ADC_MODULE_H
#define ADC_MODULE_H

#include <stdint.h>

void initADC();
int16_t readADC();

#endif