/**
 * @file adc_module.cpp
 * @brief Implementação do módulo ADC para a leitura do BPW34
 */

#include "adc_module.h"
#include "pins.h"
#include "sd_module.h"

/**
 * @brief Inicializa a comunicação Modbus com o sensor
 */
void initADC()
{
    logSerialSD("Iniciando leitura do BPW34...");
    pinMode(ADC_PIN, INPUT);
}

/**
 * @brief Realiza uma leitura ADC
 * @return Valor da irradiância solar em W/m² ou -1 em caso de erro
 */
int16_t readADC()
{
    // Incluir conversao de 0-4095 para 0-2000 W/m²
    int16_t value = analogRead(ADC_PIN);
    logSerialSD("Leitura ADC: %d", value);
    return value;
}