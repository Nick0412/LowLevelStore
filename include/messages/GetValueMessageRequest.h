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

/**
 * @brief 
 * 
 * `return_message` needs to be properly allocated for this includes all of its dependent fields
 * 
 * - GetValueMessageRequest (return_message)
 *   - AugmentedBuffer* (key) [needs to be allocated]
 *     - void* (buffer_pointer) [needs to be allocated]
 *     - int (buffer_size) [needs to be allocated]
 * 
 * @param buffer 
 * @param return_message 
 */
void deserializeGetValueMessageRequest(AugmentedBuffer* buffer, GetValueMessageRequest* return_message);

void getValueMessageRequestCalculateKeySizeOffset(AugmentedBuffer* message, uint32_t* return_key_size_offset);

void getValueMessageRequestCalculateKeyDataOffset(AugmentedBuffer* message, uint32_t* return_key_data_offset);

void getValueMessageRequestGetKeySize(AugmentedBuffer* message, uint32_t* return_key_size);

void getValueMessageRequestGetKeyData(AugmentedBuffer* message, AugmentedBuffer* return_key_data);

void getValueMessageRequestAllocateMemory(AugmentedBuffer* message, GetValueMessageRequest* return_request);

void getValueMessageRequestDestroyMemory(GetValueMessageRequest* request);

#endif
