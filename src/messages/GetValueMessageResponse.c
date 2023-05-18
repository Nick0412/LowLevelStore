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

void deserializeGetValueMessageResponse(MemoryPoolList* pool, AugmentedBuffer* buffer, GetValueMessageResponse* return_message)
{
    // Allocate for value string and size
    uint32_t value_size_offset = 8;
    uint32_t value_size;
    get32BitUintFromBuffer(buffer, value_size_offset, &value_size);
    AugmentedBuffer value_buffer;
    allocateMemoryInPool(pool, value_size, &value_buffer);
    uint32_t value_offset = value_size_offset + 4;
    getStringFromBuffer(buffer, value_offset, &value_buffer);

    // Allocate for Augmented buffer that will be attached to return_message
    AugmentedBuffer value_attach_buff;
    allocateMemoryInPool(pool, sizeof(AugmentedBuffer), &value_attach_buff);
    return_message->value = (AugmentedBuffer*)value_attach_buff.buffer_pointer;
    return_message->value->buffer_pointer = value_buffer.buffer_pointer;
    return_message->value->buffer_size = value_buffer.buffer_size;
}
