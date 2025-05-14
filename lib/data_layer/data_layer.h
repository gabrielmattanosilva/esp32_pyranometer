/**
 * @file data_layer.h
 * @brief Data layer para armazenamento temporário de leituras
 */

#ifndef DATA_LAYER_H
#define DATA_LAYER_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "rtc_module.h"

// Estrutura para armazenar uma leitura completa
typedef struct {
    DateTime timestamp;
    int16_t irradiance_pyr20;
    int16_t irradiance_bpw34;
} SolarReading;

// Estrutura do buffer circular
typedef struct {
    SolarReading *buffer;
    size_t buffer_size;
    size_t head;
    size_t tail;
    bool full;
} CircularBuffer;

/**
 * @brief Inicializa o buffer circular
 * @param cb Ponteiro para a estrutura CircularBuffer
 * @param size Tamanho do buffer
 * @return true se sucesso, false se falha
 */
bool DataLayer_init(CircularBuffer *cb, size_t size);

/**
 * @brief Adiciona uma nova leitura ao buffer (sobrescreve quando cheio)
 * @param cb Ponteiro para a estrutura CircularBuffer
 * @param reading Ponteiro para a leitura a ser adicionada
 * @return Sempre retorna true
 */
bool DataLayer_addReading(CircularBuffer *cb, const SolarReading *reading);

/**
 * @brief Obtém a leitura mais recente (sem removê-la)
 * @param cb Ponteiro para a estrutura CircularBuffer
 * @param[out] reading Ponteiro para estrutura que receberá os dados
 * @return true se há dados disponíveis, false caso contrário
 */
bool DataLayer_getLatestReading(CircularBuffer *cb, SolarReading *reading);

/**
 * @brief Verifica se o buffer está vazio
 * @param cb Ponteiro para a estrutura CircularBuffer
 * @return true se vazio, false caso contrário
 */
bool DataLayer_isEmpty(const CircularBuffer *cb);

#endif