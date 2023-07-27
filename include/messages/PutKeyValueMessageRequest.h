#ifndef PUT_KEY_VALUE_MESSAGE_REQUEST_H
#define PUT_KEY_VALUE_MESSAGE_REQUEST_H

#include <stdint.h>
#include "Common.h"
#include "common/SizeAwareBuffer.h"
#include "common/MemoryPool.h"

/**
 * Size     | Type      | Key Size  | Key String    | Value Size  | Value String
 * -----------------------------------------------------------------------------
 * 4 Bytes  | 4 Bytes   | 4 Bytes   | ?? Bytes      | 4 Bytes     | ?? Bytes
 */
typedef struct PutKeyValueMessageRequest
{
    SizeAwareBuffer key;
    SizeAwareBuffer value;

} PutKeyValueMessageRequest;

void PutKeyValueMessageRequest_CalculateSize(const PutKeyValueMessageRequest* request, uint32_t* return_request_size);

/**
 * Precondition:
 *   - `return_message_bytes` must be allocated to hold the request
*/
void PutKeyValueMessageRequest_SerializeIntoBuffer(const PutKeyValueMessageRequest* request, SizeAwareBuffer* return_message_bytes);

/**
 * Precondition:
 *   - `return_request` must be allocated to hold the deserialized bytes
*/
void PutKeyValueMessageRequest_Deserialize(const SizeAwareBuffer* message_bytes, PutKeyValueMessageRequest* return_request);

/**
 * Postcondition:
 *   - `return_message_bytes` has been allocated must be cleaned up
*/
void PutKeyValueMessageRequest_AllocateBuffer(const PutKeyValueMessageRequest* request, SizeAwareBuffer* return_message_bytes);

void PutKeyValueMessageRequest_DestroyBuffer(SizeAwareBuffer* message_bytes);

/**
 * Postcondition:
 *   - `return_request` has been allocated must be cleaned up
*/
void PutKeyValueMessageRequest_AllocateMessage(const SizeAwareBuffer* message_bytes, PutKeyValueMessageRequest* return_request);

void PutKeyValueMessageRequest_DestroyMessage(PutKeyValueMessageRequest* request);

void PutKeyValueMessageRequest_GetKeySizeOffset(const SizeAwareBuffer* message_bytes, uint32_t* return_key_size_offset);

void PutKeyValueMessageRequest_GetKeySize(const SizeAwareBuffer* message_bytes, uint32_t* return_key_size);

void PutKeyValueMessageRequest_GetKeyOffset(const SizeAwareBuffer* message_bytes, uint32_t* return_key_offset);

/**
 * Precondition:
 *   - `return_key` needs to be allocated to hold the key
*/
void PutKeyValueMessageRequest_GetKey(const SizeAwareBuffer* message_bytes, SizeAwareBuffer* return_key);

void PutKeyValueMessageRequest_GetValueSizeOffset(const SizeAwareBuffer* message_bytes, uint32_t* return_value_size_offset);

void PutKeyValueMessageRequest_GetValueSize(const SizeAwareBuffer* message_bytes, uint32_t* return_value_size);

void PutKeyValueMessageRequest_GetValueOffset(const SizeAwareBuffer* message_bytes, uint32_t* return_value_offset);

/**
 * Precondition:
 *   - `return_value` needs to be allocated to hold the value
*/
void PutKeyValueMessageRequest_GetValue(const SizeAwareBuffer* message_bytes, SizeAwareBuffer* return_value);

#endif
