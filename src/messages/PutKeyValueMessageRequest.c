#include "messages/PutKeyValueMessageRequest.h"
#include "messages/Constants.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

void calculatePutKeyValueMessageRequestSize(PutKeyValueMessageRequest* message, uint32_t* return_size)
{
    // TODO: Extract these into constants
    // Size of message -> store in 4 bytes
    // Type of message -> store in 4 bytes
    // Size of strings/variable types -> store in 4 bytes
    uint32_t temp_size = 4 + 4 + 4 + message->key->buffer_size + 4 + message->value->buffer_size;

    *return_size = temp_size;
}

void serializePutKeyValueMessageRequest(PutKeyValueMessageRequest* message, SizeAwareBuffer* buffer)
{
    // Place size of message
    uint32_t offset = 0;
    SizeAwareBuffer_Place32BitValue(buffer->buffer_size, buffer, offset);

    // Place type of message
    offset += 4;
    SizeAwareBuffer_Place32BitValue((uint32_t)PUT_KEY_VALUE_MESSAGE_REQUEST_TYPE, buffer, offset);

    // Place size of key string
    offset += 4;
    SizeAwareBuffer_Place32BitValue(message->key->buffer_size, buffer, offset);

    // Place key string
    offset += 4;
    SizeAwareBuffer_PlaceStringInBuffer(message->key, buffer, offset);

    // Place size of value string
    offset += message->key->buffer_size;
    SizeAwareBuffer_Place32BitValue(message->value->buffer_size, buffer, offset);

    // Place value string in buffer
    offset += 4;
    SizeAwareBuffer_PlaceStringInBuffer(message->value, buffer, offset);
}

void deserializePutKeyValueMessageRequest(SizeAwareBuffer* buffer, PutKeyValueMessageRequest* return_message)
{
    // Get key from buffer
    uint32_t key_size_offset = 8;
    uint32_t key_offset = key_size_offset + 4;
    uint32_t key_size;
    SizeAwareBuffer_Get32BitValue(buffer, key_size_offset, &key_size);
    // TODO: Determine if the following line needs to be here as we are expecting the `return_message`
    // key->buffer_size to be set before `deserializePutKeyValueMessageRequest` is called.
    return_message->key->buffer_size = key_size;
    SizeAwareBuffer_GetStringFromBuffer(buffer, key_offset, return_message->key);

    // Get value from buffer
    uint32_t value_size_offset = key_offset + key_size;
    uint32_t value_offset = value_size_offset + 4;
    uint32_t value_size;
    SizeAwareBuffer_Get32BitValue(buffer, value_size_offset, &value_size);
    // TODO: Same as the above TODO.
    return_message->value->buffer_size = value_size;
    SizeAwareBuffer_GetStringFromBuffer(buffer, value_offset, return_message->value);
}

void getKeySizeFromPutKeyValueBuffer(SizeAwareBuffer* buffer, uint32_t* return_key_size)
{
    uint32_t key_size_offset = 8;
    SizeAwareBuffer_Get32BitValue(buffer, key_size_offset, return_key_size);
}

void getKeyFromPutKeyValueBuffer(SizeAwareBuffer* buffer, char** return_key)
{
    uint32_t offset = 12;

    *return_key = (char*)buffer->raw_buffer + offset;
}

void getValueSizeFromPutKeyValueBuffer(SizeAwareBuffer* buffer, uint32_t* return_value_size)
{
    uint32_t value_size_offset = 12;
    uint32_t key_size;
    getKeySizeFromPutKeyValueBuffer(buffer, &key_size);
    value_size_offset += key_size;

    SizeAwareBuffer_Get32BitValue(buffer, value_size_offset, return_value_size);
}

void getValueFromPutKeyValueBuffer(SizeAwareBuffer* buffer, char** return_value)
{
    uint32_t value_offset = 12;
    uint32_t key_size;
    getKeySizeFromPutKeyValueBuffer(buffer, &key_size);
    value_offset += key_size;
    value_offset += 4;

    *return_value = (char*)buffer->raw_buffer + value_offset;
}

void calculateKeySizeOffset(SizeAwareBuffer* buffer, uint32_t* return_key_size_offset)
{
    (void)buffer;
    *return_key_size_offset = MESSAGE_DATA_OFFSET;
}

void calculateKeyOffset(SizeAwareBuffer* buffer, uint32_t* return_key_offset)
{
    calculateKeySizeOffset(buffer, return_key_offset);
    *return_key_offset += MESSAGE_SIZE_FIELD_BYTES;
}

void calculateValueSizeOffset(SizeAwareBuffer* buffer, uint32_t* return_value_size_offset)
{
    calculateKeyOffset(buffer, return_value_size_offset);
    uint32_t key_size;
    getKeySizeFromPutKeyValueBuffer(buffer, &key_size);

    *return_value_size_offset += key_size;
}

void calculateValueOffset(SizeAwareBuffer* buffer, uint32_t* return_value_offset)
{
    calculateValueSizeOffset(buffer, return_value_offset);
    *return_value_offset += MESSAGE_SIZE_FIELD_BYTES;
}

void allocateMemoryForPutKeyValueMessageRequest(PutKeyValueMessageRequest* message, SizeAwareBuffer* buffer)
{
    message->key = malloc(sizeof(SizeAwareBuffer));
    message->value = malloc(sizeof(SizeAwareBuffer));

    getKeySizeFromPutKeyValueBuffer(buffer, &message->key->buffer_size);
    getValueSizeFromPutKeyValueBuffer(buffer, &message->value->buffer_size);

    message->key->raw_buffer = malloc(message->key->buffer_size);
    message->value->raw_buffer = malloc(message->value->buffer_size);
}

void destroyMemoryForPutKeyValueMessageRequest(PutKeyValueMessageRequest* message)
{
    free(message->key->raw_buffer);
    free(message->value->raw_buffer);

    free(message->key);
    free(message->value);
}
