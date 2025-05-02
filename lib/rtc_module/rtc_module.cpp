#include <Wire.h>
#include "pins.h"
#include "rtc_module.h"

RTC_DS1307 rtc;

void initRTC()
{
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

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