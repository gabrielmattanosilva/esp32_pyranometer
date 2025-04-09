#include "rtc_module.h"
#include <Wire.h>

RTC_DS1307 rtc;

void initRTC()
{
  Wire.begin(21, 22); // SDA, SCL

  if (!rtc.begin())
  {
    Serial.println("Erro ao inicializar RTC!");
    while (1)
      ;
  }
}

DateTime getCurrentTime()
{
  return rtc.now();
}