#ifndef GET_VALUE_MESSAGE_REQUEST_H
#define GET_VALUE_MESSAGE_REQUEST_H

#include "common/MemoryPool.h"
#include "common/SizeAwareBuffer.h"

/**
 * Size     | Type      | Key Size  | Key String
 * ---------------------------------------------
 * 4 Bytes  | 4 Bytes   | 4 Bytes   | ?? Bytes
 */
typedef struct GetValueMessageRequest
{
    SizeAwareBuffer key;

} GetValueMessageRequest;

/**
 *********************************************************************
 * Rewritten section for cleanup.
 *********************************************************************
 */

void GetValueMessageRequest_CalculateSize(const GetValueMessageRequest* request, uint32_t* return_request_size);

/**
 * Precondition: 
 *   - `return_message_bytes` must be allocated to hold the request
 */
void GetValueMessageRequest_SerializeIntoBuffer(const GetValueMessageRequest* request, SizeAwareBuffer* return_message_bytes);

/**
 * Precondition:
 *   - `return_request` must be allocated to hold the deserialized message bytes
*/
void GetValueMessageRequest_Deserialize(const SizeAwareBuffer* message_bytes, GetValueMessageRequest* return_request);

/**
 * @brief Allocates the space needed inside `return_message_bytes` to store the `request`.
 * 
 * Postcondition:
 *   - `return_message_bytes` is allocated and must be cleaned up
*/
void GetValueMessageRequest_AllocateBuffer(const GetValueMessageRequest* request, SizeAwareBuffer* return_message_bytes);

void GetValueMessageRequest_DestroyBuffer(SizeAwareBuffer* message_bytes);

/**
 * Postcondition:
 *   - `return_request` is allocated and must be cleaned up
*/
void GetValueMessageRequest_AllocateMessage(const SizeAwareBuffer* message_bytes, GetValueMessageRequest* return_request);

void GetValueMessageRequest_DestroyMessage(GetValueMessageRequest* request);

void GetValueMessageRequest_GetKeySizeOffset(const SizeAwareBuffer* message_bytes, uint32_t* return_key_size_offset);

void GetValueMessageRequest_GetKeySize(const SizeAwareBuffer* message_bytes, uint32_t* return_key_size);

void GetValueMessageRequest_GetKeyOffset(const SizeAwareBuffer* message_bytes, uint32_t* return_key_offset);

/**
 * Precondition:
 *   - `return_key` must be allocated to hold the key
*/
void GetValueMessageRequest_GetKey(const SizeAwareBuffer* message_bytes, SizeAwareBuffer* return_key);

#endif
