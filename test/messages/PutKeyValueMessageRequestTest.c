#include "messages/PutKeyValueMessageRequest.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void testCalculatePutKeyMessageRequestSize()
{
    PutKeyValueMessageRequest message = {
        .key = &(AugmentedBuffer) {
            .buffer_pointer = "test_key",
            .buffer_size = 8
        },
        .value = &(AugmentedBuffer) {
            .buffer_pointer = "test_value",
            .buffer_size = 10
        }
    };

    uint32_t size;
    calculatePutKeyValueMessageRequestSize(&message, &size);
    
    assert(size == (4 + 4 + 4 + 8 + 4 + 10));
}

void testSerializeDeserializePutKeyValueMessageRequest()
{
    PutKeyValueMessageRequest message = {
        .key = &(AugmentedBuffer) {
            .buffer_pointer = "test_key",
            .buffer_size = 8
        },
        .value = &(AugmentedBuffer) {
            .buffer_pointer = "test_value",
            .buffer_size = 10
        }
    };

    // Serialize
    uint32_t message_size;
    calculatePutKeyValueMessageRequestSize(&message, &message_size);
    AugmentedBuffer serialized_buffer = {
        .buffer_pointer = malloc(message_size),
        .buffer_size = message_size
    };
    serializePutKeyValueMessageRequest(&message, &serialized_buffer);

    // Deserialize
    PutKeyValueMessageRequest result_message;
    result_message.key = malloc(sizeof(AugmentedBuffer));
    result_message.key->buffer_pointer = malloc(8);
    result_message.key->buffer_size = 8;
    result_message.value = malloc(sizeof(AugmentedBuffer));
    result_message.value->buffer_pointer = malloc(10);
    result_message.value->buffer_size = 10;
    deserializePutKeyValueMessageRequest(&serialized_buffer, &result_message);

    // Test
    assert(areAugmentedBuffersSame(result_message.key, message.key));
    assert(areAugmentedBuffersSame(result_message.value, message.value));

    // Cleanup
    free(serialized_buffer.buffer_pointer);
    free(result_message.key->buffer_pointer);
    free(result_message.key);
    free(result_message.value->buffer_pointer);
    free(result_message.value);
}

void testGetKeyAndGetKeySize()
{
    PutKeyValueMessageRequest message = {
        .key = &(AugmentedBuffer) {
            .buffer_pointer = "key1",
            .buffer_size = 4
        },
        .value = &(AugmentedBuffer) {
            .buffer_pointer = "value1",
            .buffer_size = 6
        }
    };

    // Serialize
    uint32_t message_size;
    calculatePutKeyValueMessageRequestSize(&message, &message_size);
    AugmentedBuffer serialized_buffer = {
        .buffer_pointer = malloc(message_size),
        .buffer_size = message_size
    };
    serializePutKeyValueMessageRequest(&message, &serialized_buffer);

    // Get key size
    uint32_t key_size_actual;
    getKeySizeFromPutKeyValueBuffer(&serialized_buffer, &key_size_actual);

    // Get key
    char* string_pointer_actual;
    getKeyFromPutKeyValueBuffer(&serialized_buffer, &string_pointer_actual);

    // Test
    assert(key_size_actual == 4);
    assert(memcmp(string_pointer_actual, "key1", 4) == 0);

    // Cleanup
    free(serialized_buffer.buffer_pointer);
}

void testGetValueAndGetValueSize()
{
    PutKeyValueMessageRequest message = {
        .key = &(AugmentedBuffer) {
            .buffer_pointer = "key1",
            .buffer_size = 4
        },
        .value = &(AugmentedBuffer) {
            .buffer_pointer = "value1",
            .buffer_size = 6
        }
    };

    // Serialize
    uint32_t message_size;
    calculatePutKeyValueMessageRequestSize(&message, &message_size);
    AugmentedBuffer serialized_buffer = {
        .buffer_pointer = malloc(message_size),
        .buffer_size = message_size
    };
    serializePutKeyValueMessageRequest(&message, &serialized_buffer);

    // Get value size
    uint32_t value_size_actual;
    getValueSizeFromPutKeyValueBuffer(&serialized_buffer, &value_size_actual);

    // Get value
    char* value_actual;
    getValueFromPutKeyValueBuffer(&serialized_buffer, &value_actual);

    // Test
    assert(value_size_actual == 6);
    assert(memcmp(value_actual, "value1", 6) == 0);

    // Cleanup
    free(serialized_buffer.buffer_pointer);
}

void testOffsetCalculationsPutKeyValueMessage()
{
    PutKeyValueMessageRequest message = {
        .key = &(AugmentedBuffer) {
            .buffer_pointer = "abcde",
            .buffer_size = 5
        },
        .value = &(AugmentedBuffer) {
            .buffer_pointer = "testing123",
            .buffer_size = 10
        }
    };

    // Serialize data
    uint32_t message_size;
    calculatePutKeyValueMessageRequestSize(&message, &message_size);
    AugmentedBuffer serialized_buffer = {
        .buffer_pointer = malloc(message_size),
        .buffer_size = message_size
    };
    serializePutKeyValueMessageRequest(&message, &serialized_buffer);

    uint32_t key_size_offset;
    calculateKeySizeOffset(&serialized_buffer, &key_size_offset);

    uint32_t key_offset;
    calculateKeyOffset(&serialized_buffer, &key_offset);

    uint32_t value_size_offset;
    calculateValueSizeOffset(&serialized_buffer, &value_size_offset);

    uint32_t value_offset;
    calculateValueOffset(&serialized_buffer, &value_offset);

    assert(key_size_offset == 8);
    assert(key_offset == 12);
    assert(value_size_offset == 17);
    assert(value_offset == 21);

    free(serialized_buffer.buffer_pointer);
}

int main()
{
    printf("STARTING PUT KEY VALUE MESSAGE REQUEST TEST\n");

    testCalculatePutKeyMessageRequestSize();

    testSerializeDeserializePutKeyValueMessageRequest();

    testGetKeyAndGetKeySize();

    testGetValueAndGetValueSize();

    testOffsetCalculationsPutKeyValueMessage();
}
