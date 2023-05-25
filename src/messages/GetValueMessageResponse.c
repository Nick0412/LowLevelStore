#include "messages/GetValueMessageResponse.h"
#include "messages/MessageTypes.h"

void calculateGetValueMessageResponseSize(GetValueMessageResponse* message, uint32_t* return_size)
{
    uint32_t temp_size = 4 + 4 + 4 + message->value->buffer_size;

    *return_size = temp_size;
}

void serializeGetValueMessageResponse(GetValueMessageResponse* message, AugmentedBuffer* return_buffer)
{
    // Place message size
    uint32_t offset = 0;
    place32BitUintInBuffer(return_buffer->buffer_size, return_buffer, offset);

    // Place message type
    offset += 4;
    place32BitUintInBuffer((uint32_t)GET_VALUE_MESSAGE_RESPONSE_TYPE, return_buffer, offset);

    // Place size of value message
    offset += 4;
    place32BitUintInBuffer(message->value->buffer_size, return_buffer, offset);

    // Place value string
    offset += 4;
    placeStringInBuffer(message->value, return_buffer, offset);
}

void deserializeGetValueMessageResponse(AugmentedBuffer* buffer, GetValueMessageResponse* return_message)
{
    uint32_t value_size_offset = 8;
    uint32_t value_offset = value_size_offset + 4;
    uint32_t value_size;
    get32BitUintFromBuffer(buffer, value_size_offset, &value_size);
    // Important that buffer_size here is set before the function call as the function `getStringFromBuffer`
    // relies on it to determine the string size. Consider removing this line as return_message->value->buffer_size
    // should have been set before `deserializeGetValueMessageResponse` is even called.
    return_message->value->buffer_size = value_size;
    getStringFromBuffer(buffer, value_offset, return_message->value);
}
