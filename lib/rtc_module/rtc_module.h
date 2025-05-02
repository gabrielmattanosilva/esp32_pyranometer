/**
 * @file rtc_module.h
 * @brief Interface do módulo RTC (Real-Time Clock)
 */
#ifndef RTC_MODULE_H
#define RTC_MODULE_H

#include <RTClib.h>

class RTC_DS1307;
extern RTC_DS1307 rtc; ///< Instância do RTC

/**
 * @brief Inicializa o módulo RTC
 */
void initRTC();

/**
 * @brief Obtém a data e hora atuais
 * @return Objeto DateTime com a data/hora atual
 */
DateTime getCurrentTime();

#endif