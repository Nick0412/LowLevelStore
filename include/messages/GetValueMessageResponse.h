#ifndef GET_VALUE_MESSAGE_RESPONSE_H
#define GET_VALUE_MESSAGE_RESPONSE_H

#include "common/BufferHelper.h"
#include "common/MemoryPool.h"

/**
 * Size     | Type      | Value Size  | Value String
 * -------------------------------------------------
 * 4 Bytes  | 4 Bytes   | 4 Bytes   | ?? Bytes
 */
typedef struct GetValueMessageResponse
{
    AugmentedBuffer* value;

} GetValueMessageResponse;

void calculateGetValueMessageResponseSize(GetValueMessageResponse* message, uint32_t* return_size);

void serializeGetValueMessageResponse(GetValueMessageResponse* message, AugmentedBuffer* return_buffer);

void deserializeGetValueMessageResponse(MemoryPoolList* pool, AugmentedBuffer* buffer, GetValueMessageResponse* return_message);

#endif
