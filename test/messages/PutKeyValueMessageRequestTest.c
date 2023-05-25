#include "messages/PutKeyValueMessageRequest.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

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

int main()
{
    printf("STARTING PUT KEY VALUE MESSAGE REQUEST TEST\n");

    testCalculatePutKeyMessageRequestSize();

    testSerializeDeserializePutKeyValueMessageRequest();
}
