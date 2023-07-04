#include "messages/PutKeyValueMessageResponse.h"

#include "messages/Constants.h"
#include "messages/MessageTypes.h"
#include "messages/MessageStatus.h"

void calculatePutKeyValueMessageResponseSize(PutKeyValueMessageResponse* response, uint32_t* return_size)
{
    (void)response;
    *return_size = MESSAGE_DATA_OFFSET + MESSAGE_SIZE_FIELD_BYTES;
}

void serializePutKeyValueMessageResponse(PutKeyValueMessageResponse* response, SizeAwareBuffer* return_serialized_data)
{  
    // Place message size after getting it
    uint32_t message_size;
    calculatePutKeyValueMessageResponseSize(response, &message_size);
    uint32_t offset = 0;
    SizeAwareBuffer_Place32BitValue(message_size, return_serialized_data, offset);

    // Place type of message
    offset += 4;
    SizeAwareBuffer_Place32BitValue((uint32_t)PUT_KEY_VALUE_MESSAGE_RESPONSE_TYPE, return_serialized_data, offset);

    // Place message status
    offset += 4;
    SizeAwareBuffer_Place32BitValue((uint32_t)MESSAGE_SUCCESS, return_serialized_data, offset);
}

void deserializePutKeyValueMessageResponse(SizeAwareBuffer* input_buffer, PutKeyValueMessageResponse* return_response)
{
    uint32_t message_status_num;
    SizeAwareBuffer_Get32BitValue(input_buffer, 8, &message_status_num);
    return_response->status = (MessageStatus)message_status_num;
}

