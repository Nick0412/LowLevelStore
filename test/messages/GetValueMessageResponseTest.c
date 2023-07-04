#include "messages/GetValueMessageResponse.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

void testCalculateGetValueMessageRequestSize()
{
    GetValueMessageResponse message = {
        .value = &(SizeAwareBuffer) {
            .raw_buffer = (uint8_t*)"test_value",
            .buffer_size = 10
        }
    };

    uint32_t size;
    calculateGetValueMessageResponseSize(&message, &size);
    
    assert(size == (4 + 4 + 4 + 10));
}

void testSerializeDeserializeGetValueMessageResponse()
{
    GetValueMessageResponse message = {
        .value = &(SizeAwareBuffer) {
            .raw_buffer = (uint8_t*)"test_value",
            .buffer_size = 10
        }
    };

    uint32_t message_size;
    calculateGetValueMessageResponseSize(&message, &message_size);
    SizeAwareBuffer serialized_buffer = {
        .raw_buffer = malloc(message_size),
        .buffer_size = message_size
    };
    serializeGetValueMessageResponse(&message, &serialized_buffer);

    GetValueMessageResponse result_message;
    result_message.value = malloc (sizeof(SizeAwareBuffer));
    // In the future we may want helper functions to grab the size of these fields from the serialized
    // buffer.
    result_message.value->raw_buffer = malloc(10);
    result_message.value->buffer_size = 10;
    deserializeGetValueMessageResponse(&serialized_buffer, &result_message);

    assert(SizeAwareBuffer_AreContentsSame(message.value, result_message.value));

    free(serialized_buffer.raw_buffer);
    free(result_message.value->raw_buffer);
    free(result_message.value);    
}

int main()
{
    printf("STARTING GET VALUE VALUE MESSAGE RESPONSE TEST\n");

    testCalculateGetValueMessageRequestSize();

    testSerializeDeserializeGetValueMessageResponse();
}
