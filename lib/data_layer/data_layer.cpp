/**
 * @file data_layer.cpp
 * @brief Implementação da data layer
 */

#include <stdlib.h>
#include "data_layer.h"

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

bool DataLayer_isEmpty(const CircularBuffer *cb)
{
    return (!cb->full && (cb->head == cb->tail));
}
