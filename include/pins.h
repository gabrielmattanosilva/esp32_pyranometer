/**
 * @file pins.h
 * @brief Definição dos pinos e configurações de hardware do projeto
 * @details Este arquivo centraliza todas as definições de pinos para facilitar
 *          a manutenção e garantir consistência em todo o projeto.
 */

#ifndef PINS_H
#define PINS_H

// Configurações de pinos para o módulo Modbus (UART2 do ESP32)
#define MODBUS_RX_PIN   16
#define MODBUS_TX_PIN   17

// Configurações de pinos para o RTC (I2C)
#define I2C_SDA_PIN     21
#define I2C_SCL_PIN     22

// Configurações de pinos para o módulo SD (SPI)
#define SPI_MISO_PIN    19
#define SPI_MOSI_PIN    23
#define SPI_SCK_PIN     18
#define SD_CS_PIN       5

// Outros pinos
#define LED_PIN         2    // LED interno da placa ESP32 (GPIO2)

#endif // PINS_H