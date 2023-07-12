#ifndef PUT_KEY_VALUE_MESSAGE_RESPONSE_H
#define PUT_KEY_VALUE_MESSAGE_RESPONSE_H

#include "MessageStatus.h"
#include "common/SizeAwareBuffer.h"
#include <stdint.h>

/**
 * | Size of message | Type of message | Message status |
 * | 4 Bytes         | 4 Bytes         | 4 Bytes
 * 
 */
typedef struct PutKeyValueMessageResponse
{
    MessageStatus status;

} PutKeyValueMessageResponse;

void PutKeyValueMessageResponse_CalculateSize(const PutKeyValueMessageResponse* response, uint32_t* return_response_size);

void PutKeyValueMessageResponse_SerializeIntoBuffer(const PutKeyValueMessageResponse* response, SizeAwareBuffer* return_message_bytes);

void PutKeyValueMessageResponse_Deserialize(const SizeAwareBuffer* message_bytes, PutKeyValueMessageResponse* return_response);

void PutKeyValueMessageResponse_AllocateBuffer(const PutKeyValueMessageResponse* response, SizeAwareBuffer* return_message_bytes);

void PutKeyValueMessageResponse_DestroyBuffer(SizeAwareBuffer* message_bytes);

// This function does not do anything but is here to keep the same pattern as other messages.
void PutKeyValueMessageResponse_AllocateMessage(const SizeAwareBuffer* message_bytes, PutKeyValueMessageResponse* return_response);

// This function does not do anything but is here to keep the same pattern as other messages.
void PutKeyValueMessageResponse_DestroyMessage(PutKeyValueMessageResponse* response);

void PutKeyValueMessageResponse_GetStatusOffset(const SizeAwareBuffer* message_bytes, uint32_t* return_status_offset);

void PutKeyValueMessageResponse_GetStatus(const SizeAwareBuffer* message_bytes, MessageStatus* return_status);

#endif
