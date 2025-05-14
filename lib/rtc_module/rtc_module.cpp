/**
 * @file rtc_module.cpp
 * @brief Implementação do módulo RTC (Real-Time Clock)
 */

#include <Wire.h>
#include "pins.h"
#include "rtc_module.h"

RTC_DS1307 rtc; ///< Instância global do RTC DS1307

/**
 * @brief Inicializa o módulo RTC
 */
void initRTC()
{
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    if (!rtc.begin())
    {
        Serial.println("Erro ao inicializar RTC.");
        while (1)
            ; // Loop infinito em caso de falha
    }
}

/**
 * @brief Obtém a data e hora atuais
 * @return Objeto DateTime com a data/hora atual
 */
DateTime getCurrentTime()
{
    return rtc.now();
}