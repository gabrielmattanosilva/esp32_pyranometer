/**
 * @file rtc_module.cpp
 * @brief Implementação do módulo RTC (Real-Time Clock)
 */

#include <Wire.h>
#include "pins.h"
#include "rtc_module.h"

RTC_DS1307 rtc; ///< Instância global do RTC DS1307

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

DateTime getCurrentTime()
{
	return rtc.now();
}