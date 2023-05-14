#include "messages/PutKeyValueMessageRequest.h"
#include <stdio.h>
#include <assert.h>

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
    MemoryPoolList pool;
    initializeMemoryPool(&pool);

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

    uint32_t message_size;
    calculatePutKeyValueMessageRequestSize(&message, &message_size);
    AugmentedBuffer buff;
    allocateMemoryInPool(&pool, message_size, &buff); // buff.buffer_pointer will hold message
    serializePutKeyValueMessageRequest(&message, &buff);
    PutKeyValueMessageRequest result_message;
    deserializePutKeyValueMessageRequest(&pool, &buff, &result_message);

    assert(areAugmentedBuffersSame(result_message.key, message.key));
    assert(areAugmentedBuffersSame(result_message.value, message.value));

    destroyMemoryPool(&pool);
}

int main()
{
    printf("STARTING PUT KEY VALUE MESSAGE REQUEST TEST\n");

    testCalculatePutKeyMessageRequestSize();

    testSerializeDeserializePutKeyValueMessageRequest();
}
