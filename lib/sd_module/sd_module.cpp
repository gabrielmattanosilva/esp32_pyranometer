/**
 * @file sd_module.cpp
 * @brief Implementação do módulo para armazenamento em cartão SD
 */

#include <SPI.h>
#include "pins.h"
#include "rtc_module.h"
#include "sd_module.h"

File sdFile; ///< Arquivo atualmente aberto no SD

/**
 * @brief Inicializa o módulo SD
 */
void initSD()
{
    SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SD_CS_PIN);

    if (!SD.begin(SD_CS_PIN))
    {
        Serial.println("Erro ao inicializar cartão SD.");
        while (1)
            ; // Loop infinito em caso de falha
    }
}

/**
 * @brief Gera nome do arquivo de log baseado na data
 * @return String com o nome do arquivo
 */
String generateLogFileName()
{
    DateTime now = getCurrentTime();
    static char fileName[32];
    snprintf(fileName, sizeof(fileName), "/log_%04d-%02d-%02d.txt",
             now.year(), now.month(), now.day());

    return String(fileName);
}

/**
 * @brief Gera nome do arquivo de dados baseado na data
 * @return String com o nome do arquivo
 */
String generateDataFileName()
{
    DateTime now = getCurrentTime();
    static char fileName[32];
    snprintf(fileName, sizeof(fileName), "/data_%04d-%02d-%02d.txt",
             now.year(), now.month(), now.day());

    return String(fileName);
}

/**
 * @brief Registra uma mensagem no log do sistema
 * @param format String de formato (printf-style)
 * @param ... Argumentos variádicos
 */
void logSerialSD(const char *format, ...)
{
    static char logMessage[256];
    va_list args;
    va_start(args, format);
    vsnprintf(logMessage, sizeof(logMessage), format, args);
    va_end(args);

    DateTime now = getCurrentTime();
    unsigned long milliseconds = millis() % 1000;
    char timestamp[32];
    snprintf(timestamp, sizeof(timestamp), "%04d-%02d-%02d %02d:%02d:%02d.%03lu",
             now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second(), milliseconds);

    String logFileName = generateLogFileName();
    sdFile = SD.open(logFileName.c_str(), FILE_APPEND);

    if (sdFile)
    {
        sdFile.printf("[%s] %s\n", timestamp, logMessage);
        sdFile.close();
    }
    else
    {
        Serial.println("Erro ao abrir arquivo de log no cartão SD.");
    }

    Serial.printf("[%s] %s\n", timestamp, logMessage);
}

/**
 * @brief Escreve dados de irradiância no arquivo de dados
 * @param irradiance_pyr20 Valor da irradiância solar do PYR20
 * @param irradiance_bpw34 Valor da irradiância solar do BPW34
 * @return true se bem-sucedido, false caso contrário
 */
bool writeDataToSD(int16_t irradiance_pyr20, int16_t irradiance_bpw34)
{
    DateTime now = getCurrentTime();
    static char dataLine[64];
    snprintf(dataLine, sizeof(dataLine), "%02d-%02d-%04d %02d:%02d:%02d, %d, %d",
             now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second(),
             irradiance_pyr20, irradiance_bpw34);

    String dataFileName = generateDataFileName();
    sdFile = SD.open(dataFileName.c_str(), FILE_APPEND);

    if (!sdFile)
    {
        return false;
    }

    sdFile.println(dataLine);
    sdFile.close();

    return true;
}