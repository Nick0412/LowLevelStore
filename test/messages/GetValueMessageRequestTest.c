#include "messages/GetValueMessageRequest.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

void testCalculateGetValueMessageRequestSize()
{
    GetValueMessageRequest message = {
        .key = &(AugmentedBuffer) {
            .buffer_pointer = "test_key",
            .buffer_size = 8
        }
    };

    uint32_t size;
    calculateGetValueMessageRequestSize(&message, &size);

    assert(size == (4 + 4 + 4 + 8));
}

void testSerializeDeserializeGetValueMessageRequest()
{
    GetValueMessageRequest message = {
        .key = &(AugmentedBuffer) {
            .buffer_pointer = "test_key",
            .buffer_size = 8
        }
    };

    // Get message size
    uint32_t message_size;
    calculateGetValueMessageRequestSize(&message, &message_size);

    // Create a buffer that can hold the message size.
    AugmentedBuffer serialized_buffer;
    serialized_buffer.buffer_pointer = malloc(message_size);
    serialized_buffer.buffer_size = message_size;
    serializeGetValueMessageRequest(&message, &serialized_buffer);

    // Create a result message that can hold all the deserialized data.
    GetValueMessageRequest result_message;
    result_message.key = malloc(sizeof(AugmentedBuffer));
    result_message.key->buffer_pointer = malloc(8);
    result_message.key->buffer_size = 8;
    deserializeGetValueMessageRequest(&serialized_buffer, &result_message);

    assert(areAugmentedBuffersSame(result_message.key, message.key));
    
    free(result_message.key->buffer_pointer);
    free(result_message.key);
    free(serialized_buffer.buffer_pointer);
}

int main()
{
    printf("STARTING GET VALUE VALUE MESSAGE REQUEST TEST\n");
    testCalculateGetValueMessageRequestSize();

    testSerializeDeserializeGetValueMessageRequest();
}
