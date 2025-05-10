/**
 * @file modbus_module.cpp
 * @brief Implementação do módulo Modbus para comunicação com o sensor PYR20
 */

#include <HardwareSerial.h>
#include "modbus_module.h"
#include "pins.h"
#include "sd_module.h"

HardwareSerial modbusSerial(2); ///< Objeto Serial para comunicação Modbus

/**
 * @brief Inicializa a comunicação Modbus com o sensor
 */
void initModbus()
{
    modbusSerial.begin(9600, SERIAL_8N1, MODBUS_RX_PIN, MODBUS_TX_PIN);
    logSerialSD("Iniciando leitura do PYR20...");
}

/**
 * @brief Calcula o CRC para uma mensagem Modbus
 * @param data Ponteiro para os dados da mensagem
 * @param length Comprimento dos dados
 * @return Valor CRC calculado
 */
uint16_t check_crc(const uint8_t *data, uint8_t length)
{
    uint16_t crc = 0xFFFF;

    for (uint8_t i = 0; i < length; i++)
    {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc;
}

/**
 * @brief Envia uma requisição Modbus para o sensor
 */
void modbus_tx()
{
    static const uint8_t modbus_function[] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x01, 0x31, 0xCA};

    while (modbusSerial.available())
    {
        modbusSerial.read();
    }

    modbusSerial.write(modbus_function, sizeof(modbus_function));
    delay(10); ///< Pequeno delay para garantir o envio completo
}

/**
 * @brief Recebe e processa a resposta do sensor
 * @return Valor lido ou -1 em caso de timeout ou erro CRC
 */
int16_t modbus_rx()
{
    uint8_t response[7];
    int i = 0;
    unsigned long startTime = millis();

    // Aguarda resposta
    while (modbusSerial.available() == 0)
    {
        if (millis() - startTime > 100)
        {
            logSerialSD("Timeout na resposta!");
            return -1;
        }
    }

    // Lê os dados da resposta
    while (i < 7)
    {
        if (millis() - startTime > 100)
        {
            logSerialSD("Timeout na resposta!");
            return -1;
        }
        if (modbusSerial.available())
        {
            response[i++] = modbusSerial.read();
        }
    }

    // Verifica CRC
    uint16_t crc_response = (response[6] << 8) | response[5];
    uint16_t crc_computed = check_crc(response, 5);

    if (crc_response != crc_computed)
    {
        logSerialSD("Erro de CRC: esperado %04X, recebido %04X", crc_computed, crc_response);
        return -1;
    }

    return (response[3] << 8) | response[4]; ///< Retorna o valor da irradiância
}

/**
 * @brief Realiza uma leitura completa do sensor Modbus
 * @return Valor da irradiância solar em W/m² ou -1 em caso de erro
 */
int16_t readModbusData()
{
    modbus_tx();
    int16_t solar_irradiance = modbus_rx();
    logSerialSD("Irradiancia: %d W/m^2", solar_irradiance);
    return solar_irradiance;
}