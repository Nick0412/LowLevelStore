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

/**
 * @brief Deserializes a byte buffer `buffer` into a get value message response `return_message`.
 * 
 * This function expects `return_message` to be properly allocated and the size of the buffer to be set
 * before hand.
 * 
 * - GetValueMessageResponse (return_message)
 *   - AugmentedBuffer* (buffer) [needs to be allocated]
 *     - void* (buffer_pointer) [needs to be allocated]
 *     - uint32_t (buffer_size) [needs to be allocated and set to the expected size]
 * 
 * @param buffer 
 * @param return_message 
 */
void deserializeGetValueMessageResponse(AugmentedBuffer* buffer, GetValueMessageResponse* return_message);

#endif
