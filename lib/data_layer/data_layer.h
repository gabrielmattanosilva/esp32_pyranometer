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


bool DataLayer_init(CircularBuffer *cb, size_t size);
bool DataLayer_addReading(CircularBuffer *cb, const SolarReading *reading);
bool DataLayer_getLatestReading(CircularBuffer *cb, SolarReading *reading);
bool DataLayer_isEmpty(const CircularBuffer *cb);

#endif