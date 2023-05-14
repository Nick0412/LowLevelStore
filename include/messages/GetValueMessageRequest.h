#ifndef GET_VALUE_MESSAGE_REQUEST_H
#define GET_VALUE_MESSAGE_REQUEST_H

#include "common/BufferHelper.h"
#include "common/MemoryPool.h"

/**
 * Size     | Type      | Key Size  | Key String
 * ---------------------------------------------
 * 4 Bytes  | 4 Bytes   | 4 Bytes   | ?? Bytes
 */
typedef struct GetValueMessageRequest
{
    AugmentedBuffer* key;

} GetValueMessageRequest;

void calculateGetValueMessageRequestSize(GetValueMessageRequest* message, uint32_t* return_size);

void serializeGetValueMessageRequest(GetValueMessageRequest* message, AugmentedBuffer* return_buffer);

void deserializeGetValueMessageRequest(MemoryPoolList* pool, AugmentedBuffer* buffer, GetValueMessageRequest* return_message);

#endif
