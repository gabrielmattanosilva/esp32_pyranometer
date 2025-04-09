#ifndef RTC_MODULE_H
#define RTC_MODULE_H

#include <RTClib.h>

class RTC_DS1307;
extern RTC_DS1307 rtc;

void initRTC();
DateTime getCurrentTime();

#endif