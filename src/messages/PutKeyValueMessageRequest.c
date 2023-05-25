#include "messages/PutKeyValueMessageRequest.h"

void calculatePutKeyValueMessageRequestSize(PutKeyValueMessageRequest* message, uint32_t* return_size)
{
    // TODO: Extract these into constants
    // Size of message -> store in 4 bytes
    // Type of message -> store in 4 bytes
    // Size of strings/variable types -> store in 4 bytes
    uint32_t temp_size = 4 + 4 + 4 + message->key->buffer_size + 4 + message->value->buffer_size;

    *return_size = temp_size;
}

void serializePutKeyValueMessageRequest(PutKeyValueMessageRequest* message, AugmentedBuffer* buffer)
{
    // Place size of message
    uint32_t offset = 0;
    place32BitUintInBuffer(buffer->buffer_size, buffer, offset);

    // Place type of message
    offset += 4;
    place32BitUintInBuffer((uint32_t)PUT_KEY_VALUE_MESSAGE_REQUEST_TYPE, buffer, offset);

    // Place size of key string
    offset += 4;
    place32BitUintInBuffer(message->key->buffer_size, buffer, offset);

    // Place key string
    offset += 4;
    placeStringInBuffer(message->key, buffer, offset);

    // Place size of value string
    offset += message->key->buffer_size;
    place32BitUintInBuffer(message->value->buffer_size, buffer, offset);

    // Place value string in buffer
    offset += 4;
    placeStringInBuffer(message->value, buffer, offset);
}

void deserializePutKeyValueMessageRequest(AugmentedBuffer* buffer, PutKeyValueMessageRequest* return_message)
{
    // Get key from buffer
    uint32_t key_size_offset = 8;
    uint32_t key_offset = key_size_offset + 4;
    uint32_t key_size;
    get32BitUintFromBuffer(buffer, key_size_offset, &key_size);
    // TODO: Determine if the following line needs to be here as we are expecting the `return_message`
    // key->buffer_size to be set before `deserializePutKeyValueMessageRequest` is called.
    return_message->key->buffer_size = key_size;
    getStringFromBuffer(buffer, key_offset, return_message->key);

    // Get value from buffer
    uint32_t value_size_offset = key_offset + key_size;
    uint32_t value_offset = value_size_offset + 4;
    uint32_t value_size;
    get32BitUintFromBuffer(buffer, value_size_offset, &value_size);
    // TODO: Same as the above TODO.
    return_message->value->buffer_size = value_size;
    getStringFromBuffer(buffer, value_offset, return_message->value);
}
