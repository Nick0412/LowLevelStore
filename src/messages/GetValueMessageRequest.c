#include "messages/GetValueMessageRequest.h"
#include "messages/MessageTypes.h"
#include <stdio.h>

void calculateGetValueMessageRequestSize(GetValueMessageRequest* message, uint32_t* return_size)
{
    uint32_t temp_size = 4 + 4 + 4 + message->key->buffer_size;

    *return_size = temp_size;
}

void serializeGetValueMessageRequest(GetValueMessageRequest* message, AugmentedBuffer* return_buffer)
{
    // Place message size
    uint32_t offset = 0;
    place32BitUintInBuffer(return_buffer->buffer_size, return_buffer, offset);

    // Place type of message
    offset += 4;
    place32BitUintInBuffer((uint32_t)GET_VALUE_MESSAGE_REQUEST_TYPE, return_buffer, offset);

    // Place size of key string
    offset += 4;
    place32BitUintInBuffer(message->key->buffer_size, return_buffer, offset);

    // Place key string
    offset += 4;
    placeStringInBuffer(message->key, return_buffer, offset);
}

void deserializeGetValueMessageRequest(MemoryPoolList* pool, AugmentedBuffer* buffer, GetValueMessageRequest* return_message)
{
    // Read key string and size
    uint32_t key_size_offset = 8;
    uint32_t key_size;
    get32BitUintFromBuffer(buffer, key_size_offset, &key_size);
    AugmentedBuffer key_buff;
    allocateMemoryInPool(pool, key_size, &key_buff); // Allocated string size for key
    uint32_t key_offset = key_size_offset + 4;
    getStringFromBuffer(buffer, key_offset, &key_buff);

    // Allocate augmented buffer to attach to return message 
    AugmentedBuffer key_attach_buff;
    allocateMemoryInPool(pool, sizeof(AugmentedBuffer), &key_attach_buff); // Allocate the key augmented buffer for the return
    return_message->key = (AugmentedBuffer*)key_attach_buff.buffer_pointer;
    return_message->key->buffer_pointer = key_buff.buffer_pointer;
    return_message->key->buffer_size = key_buff.buffer_size;
}
