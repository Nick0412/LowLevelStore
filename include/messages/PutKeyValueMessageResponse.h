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

void calculatePutKeyValueMessageResponseSize(PutKeyValueMessageResponse* response, uint32_t* return_size);

/**
 * @brief Serializes the response message into the augmented buffer. This assumes the augmented buffer has
 * enough space allocated for it.
 * 
 * @param response 
 * @param return_serialized_data 
 */
void serializePutKeyValueMessageResponse(PutKeyValueMessageResponse* response, SizeAwareBuffer* return_serialized_data);

void deserializePutKeyValueMessageResponse(SizeAwareBuffer* input_buffer, PutKeyValueMessageResponse* return_response);

#endif
