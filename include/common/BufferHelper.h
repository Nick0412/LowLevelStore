#ifndef BUFFER_HELPER_H
#define BUFFER_HELPER_H

#include <cstdint>

typedef struct AugmentedBuffer
{
    void* buffer_pointer;
    uint32_t buffer_size;
} AugmentedBuffer;

void placeStringInBuffer(const char* string, uint32_t string_size, uint32_t offset, AugmentedBuffer* buffer);

void place32IntInBuffer(int32_t value, uint32_t offset, AugmentedBuffer* buffer);

#endif
