#include "messages/GetValueMessageRequest.h"
#include <stdio.h>
#include <assert.h>

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
    MemoryPoolList pool;
    initializeMemoryPool(&pool);

    GetValueMessageRequest message = {
        .key = &(AugmentedBuffer) {
            .buffer_pointer = "test_key",
            .buffer_size = 8
        }
    };

    uint32_t message_size;
    calculateGetValueMessageRequestSize(&message, &message_size);
    AugmentedBuffer buffer;
    allocateMemoryInPool(&pool, message_size, &buffer);
    serializeGetValueMessageRequest(&message, &buffer);
    GetValueMessageRequest result_message;
    deserializeGetValueMessageRequest(&pool, &buffer, &result_message);

    assert(areAugmentedBuffersSame(result_message.key, message.key));
    
    destroyMemoryPool(&pool);
}

int main()
{
    printf("STARTING GET VALUE VALUE MESSAGE REQUEST TEST\n");

    testCalculateGetValueMessageRequestSize();

    testSerializeDeserializeGetValueMessageRequest();
}
