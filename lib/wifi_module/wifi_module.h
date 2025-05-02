/**
 * @file wifi_module.h
 * @brief Interface do módulo WiFi
 */
#ifndef WIFI_MODULE_H
#define WIFI_MODULE_H

#include <stdint.h>

/**
 * @brief Inicializa a conexão WiFi
 */
void initWiFi();

/**
 * @brief Verifica o status da conexão WiFi
 * @return true se conectado, false caso contrário
 */
bool isWiFiConnected();

#endif