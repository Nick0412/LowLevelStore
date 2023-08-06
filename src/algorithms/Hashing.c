#include "algorithms/Hashing.h"

uint64_t Hashing_Djb2(const VoidBuffer* buffer)
{
    uint8_t* casted_buffer = buffer->data;
    uint64_t hash = 5381;

    for (uint32_t i = 0; i < buffer->number_of_bytes; i++)
    {
        hash = ((hash << 5) + hash) + casted_buffer[i];
    }

    return hash;
}
