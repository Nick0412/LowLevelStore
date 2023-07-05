#ifndef GET_VALUE_MESSAGE_RESPONSE_H
#define GET_VALUE_MESSAGE_RESPONSE_H

#include "common/SizeAwareBuffer.h"
#include "common/MemoryPool.h"

/**
 * Size     | Type      | Value Size  | Value String
 * -------------------------------------------------
 * 4 Bytes  | 4 Bytes   | 4 Bytes     | ?? Bytes
 */
typedef struct GetValueMessageResponse
{
    SizeAwareBuffer value;

} GetValueMessageResponse;

void GetValueMessageResponse_CalculateSize(const GetValueMessageResponse* response, uint32_t* return_size);

/**
 * Precondition: 
 *   - `return_message_bytes` must be allocated to hold the response
 */
void GetValueMessageResponse_SerializeIntoBuffer(const GetValueMessageResponse* response, SizeAwareBuffer* return_message_bytes);

/**
 * Precondition:
 *   - `return_response` must be allocated to hold the deserialized message bytes
*/
void GetValueMessageResponse_Deserialize(const SizeAwareBuffer* message_bytes, GetValueMessageResponse* return_response);

/**
 * Postcondition:
 *   - `return_message_bytes` is allocated and must be cleaned up
*/
void GetValueMessageResponse_AllocateBuffer(const GetValueMessageResponse* response, SizeAwareBuffer* return_message_bytes);

void GetValueMessageResponse_DestroyBuffer(SizeAwareBuffer* message_bytes);

/**
 * Postcondition:
 *   - `return_response` is allocated and must be cleaned up
*/
void GetValueMessageResponse_AllocateMessage(const SizeAwareBuffer* message_bytes, GetValueMessageResponse* return_response);

void GetValueMessageResponse_DestroyMessage(GetValueMessageResponse* response);

void GetValueMessageResponse_GetValueSizeOffset(const SizeAwareBuffer* message_bytes, uint32_t* return_value_size_offset);

void GetValueMessageResponse_GetValueSize(const SizeAwareBuffer* message_bytes, uint32_t* return_value_size);

void GetValueMessageResponse_GetValueOffset(const SizeAwareBuffer* message_bytes, uint32_t* return_value_offset);

/**
 * Precondition:
 *   - `return_value` must be allocated to hold the value
*/
void GetValueMessageResponse_GetValue(const SizeAwareBuffer* message_bytes, SizeAwareBuffer* return_value);

#endif
