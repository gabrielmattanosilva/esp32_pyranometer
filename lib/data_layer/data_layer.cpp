/**
 * @file data_layer.cpp
 * @brief Implementação da data layer
 */

#include <stdlib.h>
#include "data_layer.h"

/**
 * @brief Inicializa o buffer circular
 * @param cb Ponteiro para a estrutura CircularBuffer
 * @param size Tamanho do buffer
 * @return true se sucesso, false se falha
 */
bool DataLayer_init(CircularBuffer *cb, size_t size)
{
    cb->buffer = (SolarReading *)malloc(size * sizeof(SolarReading));
    if (!cb->buffer)
    {
        return false;
    }

    cb->buffer_size = size;
    cb->head = 0;
    cb->tail = 0;
    cb->full = false;

    return true;
}

/**
 * @brief Adiciona uma nova leitura ao buffer (sobrescreve quando cheio)
 * @param cb Ponteiro para a estrutura CircularBuffer
 * @param reading Ponteiro para a leitura a ser adicionada
 * @return Sempre retorna true
 */
bool DataLayer_addReading(CircularBuffer *cb, const SolarReading *reading)
{
    cb->buffer[cb->head] = *reading;

    if (cb->full)
    {
        cb->tail = (cb->tail + 1) % cb->buffer_size;
    }

    cb->head = (cb->head + 1) % cb->buffer_size;
    cb->full = (cb->head == cb->tail);

    return true;
}

/**
 * @brief Obtém a leitura mais recente (sem removê-la)
 * @param cb Ponteiro para a estrutura CircularBuffer
 * @param[out] reading Ponteiro para estrutura que receberá os dados
 * @return true se há dados disponíveis, false caso contrário
 */
bool DataLayer_getLatestReading(CircularBuffer *cb, SolarReading *reading)
{
    if (DataLayer_isEmpty(cb))
    {
        return false;
    }

    size_t index = (cb->head == 0) ? cb->buffer_size - 1 : cb->head - 1;
    *reading = cb->buffer[index];

    return true;
}

/**
 * @brief Verifica se o buffer está vazio
 * @param cb Ponteiro para a estrutura CircularBuffer
 * @return true se vazio, false caso contrário
 */
bool DataLayer_isEmpty(const CircularBuffer *cb)
{
    return (!cb->full && (cb->head == cb->tail));
}
