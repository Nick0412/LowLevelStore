#include "common/BufferHelper.h"
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>

void placeStringInBuffer(AugmentedBuffer* string, AugmentedBuffer* buffer, uint32_t offset)
{
    // Casting to char* allows for easier pointer arithmetic using 1 byte values.
    char* casted_buffer = (char*)buffer->buffer_pointer;

    // TODO: Figure out what error to return if the string goes outside of the buffer size.
    memcpy(casted_buffer + offset, string->buffer_pointer, string->buffer_size);
}

void place32BitUintInBuffer(uint32_t value, AugmentedBuffer* buffer, uint32_t offset)
{
    char* casted_buffer = (char*)buffer->buffer_pointer;
    uint32_t network_order_value = htonl(value);

    casted_buffer[offset] = network_order_value & 0xFF;
    casted_buffer[offset + 1] = (network_order_value >> 8) & 0xFF;
    casted_buffer[offset + 2] = (network_order_value >> 16) & 0xFF;
    casted_buffer[offset + 3] = (network_order_value >> 24) & 0xFF;
}

void get32BitUintFromBuffer(AugmentedBuffer* buffer, uint32_t offset, uint32_t* return_value)
{
    char* casted_buffer = (char*)buffer->buffer_pointer;

    // Needed to bit and with 0xFF to zero out other bits.
    *return_value = (casted_buffer[offset + 3] & 0xFF) << 24 | 
                    (casted_buffer[offset + 2] & 0xFF) << 16 | 
                    (casted_buffer[offset + 1] & 0xFF) << 8 |
                    (casted_buffer[offset] & 0xFF);

    *return_value = ntohl(*return_value);
}

void getStringFromBuffer(AugmentedBuffer* buffer, uint32_t offset, AugmentedBuffer* return_string)
{
    char* casted_buffer = (char*)buffer->buffer_pointer;

    memcpy(return_string->buffer_pointer, casted_buffer + offset, return_string->buffer_size);
}

bool areAugmentedBuffersSame(const AugmentedBuffer* first, const AugmentedBuffer* second)
{
    return (first->buffer_size == second->buffer_size) &&
           (memcmp(first->buffer_pointer, second->buffer_pointer, first->buffer_size) == 0);
}
