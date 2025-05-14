/**
 * @file rtc_module.h
 * @brief Interface do módulo RTC (Real-Time Clock)
 */
#ifndef RTC_MODULE_H
#define RTC_MODULE_H

#include <RTClib.h>

class RTC_DS1307;
extern RTC_DS1307 rtc; ///< Instância do RTC

void initRTC();
DateTime getCurrentTime();

#endif