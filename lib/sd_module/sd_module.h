/**
 * @file sd_module.h
 * @brief Interface do módulo para armazenamento em cartão SD
 */
#ifndef SD_MODULE_H
#define SD_MODULE_H

#include <SD.h>
#include <stdbool.h>
#include <stdint.h>

extern File sdFile; ///< Arquivo atual no SD

/**
 * @brief Inicializa o módulo SD
 */
void initSD();

/**
 * @brief Registra uma mensagem no log do sistema
 * @param format String de formato (printf-style)
 * @param ... Argumentos variádicos
 */
void logSerialSD(const char *format, ...);

/**
 * @brief Escreve dados de irradiância no arquivo de dados
 * @param irradiance_pyr20 Valor da irradiância solar do PYR20
 * @param irradiance_bpw34 Valor da irradiância solar do BPW34
 * @return true se bem-sucedido, false caso contrário
 */
bool writeDataToSD(int16_t irradiance_pyr20, int16_t irradiance_bpw34);

/**
 * @brief Gera nome do arquivo de log baseado na data
 * @return String com o nome do arquivo
 */
String generateLogFileName();

/**
 * @brief Gera nome do arquivo de dados baseado na data
 * @return String com o nome do arquivo
 */
String generateDataFileName();

#endif