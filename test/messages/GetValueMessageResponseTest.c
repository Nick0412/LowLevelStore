#include "messages/GetValueMessageResponse.h"
#include <stdio.h>
#include <assert.h>

void testCalculateGetValueMessageRequestSize()
{
    GetValueMessageResponse message = {
        .value = &(AugmentedBuffer) {
            .buffer_pointer = "test_value",
            .buffer_size = 10
        }
    };

    uint32_t size;
    calculateGetValueMessageResponseSize(&message, &size);
    
    assert(size == (4 + 4 + 4 + 10));
}

void testSerializeDeserializeGetValueMessageResponse()
{
    MemoryPoolList pool;
    initializeMemoryPool(&pool);

    GetValueMessageResponse message = {
        .value = &(AugmentedBuffer) {
            .buffer_pointer = "test_value",
            .buffer_size = 10
        }
    };

    uint32_t message_size;
    calculateGetValueMessageResponseSize(&message, &message_size);
    AugmentedBuffer buffer;
    allocateMemoryInPool(&pool, message_size, &buffer);
    serializeGetValueMessageResponse(&message, &buffer);
    GetValueMessageResponse result_message;
    deserializeGetValueMessageResponse(&pool, &buffer, &result_message);

    assert(areAugmentedBuffersSame(result_message.value, message.value));
    
    destroyMemoryPool(&pool);
}

int main()
{
    printf("STARTING GET VALUE VALUE MESSAGE RESPONSE TEST\n");

    testCalculateGetValueMessageRequestSize();

    testSerializeDeserializeGetValueMessageResponse();
}
