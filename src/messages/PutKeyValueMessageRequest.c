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

// TODO: This function seems like it could be cleaned up but I am unsure how. To avoid memory allocations in here,
// "return_message" already needs to be allocated along with all of its dependent members. But its dependent members
// rely on the arguments in the buffer.
void deserializePutKeyValueMessageRequest(MemoryPoolList* pool, AugmentedBuffer* buffer, PutKeyValueMessageRequest* return_message)
{
    // Read key string and size
    uint32_t key_size_offset = 8;
    uint32_t key_size;
    get32BitUintFromBuffer(buffer, key_size_offset, &key_size); // Store key size
    AugmentedBuffer key_buff;
    allocateMemoryInPool(pool, key_size, &key_buff); // Allocation: a buffer that holds key string
    uint32_t key_offset = key_size_offset + 4;
    getStringFromBuffer(buffer, key_offset, &key_buff);

    // Read value string and size
    uint32_t value_size_offset = key_offset + key_buff.buffer_size;
    uint32_t value_size;
    get32BitUintFromBuffer(buffer, value_size_offset, &value_size);
    AugmentedBuffer value_buff;
    allocateMemoryInPool(pool, value_size, &value_buff); // Allocation: a buffer that holds value string
    uint32_t value_offset = value_size_offset + 4;
    getStringFromBuffer(buffer, value_offset, &value_buff);

    // Allocate key augmented buffer to attach to `return_message`
    AugmentedBuffer key_attach_buff;
    allocateMemoryInPool(pool, sizeof(AugmentedBuffer), &key_attach_buff); // Allocation: AugmentedBuffer struct
    return_message->key = (AugmentedBuffer*)key_attach_buff.buffer_pointer;
    return_message->key->buffer_pointer = key_buff.buffer_pointer;
    return_message->key->buffer_size = key_buff.buffer_size;

    // Allocate value augmented buffer to attach to `return_message`
    AugmentedBuffer value_attach_buff;
    allocateMemoryInPool(pool, sizeof(AugmentedBuffer), &value_attach_buff); // Allocation: AugmentedBuffer struct
    return_message->value = (AugmentedBuffer*)value_attach_buff.buffer_pointer; 
    return_message->value->buffer_pointer = value_buff.buffer_pointer;
    return_message->value->buffer_size = value_buff.buffer_size;
}
