#include "messages/GetValueMessageRequest.h"
#include "messages/MessageTypes.h"
#include "messages/Constants.h"
#include <stdio.h>
#include <stdlib.h>

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

void deserializeGetValueMessageRequest(AugmentedBuffer* buffer, GetValueMessageRequest* return_message)
{   
    uint32_t key_size_offset = 8;
    uint32_t key_offset = key_size_offset + 4;
    uint32_t key_size;
    get32BitUintFromBuffer(buffer, key_size_offset, &key_size);
    return_message->key->buffer_size = key_size;
    getStringFromBuffer(buffer, key_offset, return_message->key);
}

void getValueMessageRequestCalculateKeySizeOffset(AugmentedBuffer* message, uint32_t* return_key_size_offset)
{
    *return_key_size_offset = MESSAGE_DATA_OFFSET;
}

void getValueMessageRequestCalculateKeyDataOffset(AugmentedBuffer* message, uint32_t* return_key_data_offset)
{
    uint32_t key_size_offset;
    getValueMessageRequestCalculateKeySizeOffset(message, &key_size_offset);

    *return_key_data_offset = key_size_offset + MESSAGE_SIZE_FIELD_BYTES;
}

void getValueMessageRequestGetKeySize(AugmentedBuffer* message, uint32_t* return_key_size)
{
    uint32_t key_size_offset;
    getValueMessageRequestCalculateKeySizeOffset(message, &key_size_offset);

    get32BitUintFromBuffer(message, key_size_offset, return_key_size);
}

void getValueMessageRequestGetKeyData(AugmentedBuffer* message, AugmentedBuffer* return_key_data)
{
    // Place key size into the augmented buffer field: buffer_size
    getValueMessageRequestGetKeySize(message, &return_key_data->buffer_size);

    uint32_t key_data_offset;
    getValueMessageRequestCalculateKeyDataOffset(message, &key_data_offset);
    getStringFromBuffer(message, key_data_offset, return_key_data);
}

void getValueMessageRequestAllocateMemory(AugmentedBuffer* message, GetValueMessageRequest* return_request)
{
    return_request->key = malloc(sizeof(AugmentedBuffer));

    getValueMessageRequestGetKeySize(message, &return_request->key->buffer_size);

    return_request->key->buffer_pointer = malloc(return_request->key->buffer_size);
}

void getValueMessageRequestDestroyMemory(GetValueMessageRequest* request)
{
    free(request->key->buffer_pointer);
    free(request->key);
}
