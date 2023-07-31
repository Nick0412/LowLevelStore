#include "messages/PutKeyValueMessageRequest.h"
#include "messages/Common.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static void testSize()
{
    PutKeyValueMessageRequest req = {
        .key = {
            .raw_buffer = (uint8_t*)"test-key",
            .buffer_size = 8
        },
        .value = {
            .raw_buffer = (uint8_t*)"test-value",
            .buffer_size = 10
        }
    };

    uint32_t size;
    PutKeyValueMessageRequest_CalculateSize(&req, &size);

    assert(size == (
        MESSAGE_SIZE_BYTE_SIZE + MESSAGE_TYPE_BYTE_SIZE +
        MESSAGE_SIZE_FIELD_BYTES + req.key.buffer_size +
        MESSAGE_SIZE_FIELD_BYTES + req.value.buffer_size));
}

static void testSerailizedOffsets()
{
    PutKeyValueMessageRequest req = {
        .key = {
            .raw_buffer = (uint8_t*)"test-key",
            .buffer_size = 8
        },
        .value = {
            .raw_buffer = (uint8_t*)"test-value",
            .buffer_size = 10
        }
    };

    uint32_t size;
    PutKeyValueMessageRequest_CalculateSize(&req, &size);

    SizeAwareBuffer buffer;
    PutKeyValueMessageRequest_AllocateBuffer(&req, &buffer);
    PutKeyValueMessageRequest_SerializeIntoBuffer(&req, &buffer);
    
    uint32_t key_size_offset;
    uint32_t key_size;
    uint32_t value_size_offset;
    uint32_t value_size;
    PutKeyValueMessageRequest_GetKeySizeOffset(&buffer, &key_size_offset);
    PutKeyValueMessageRequest_GetKeySize(&buffer, &key_size);
    PutKeyValueMessageRequest_GetValueSizeOffset(&buffer, &value_size_offset);
    PutKeyValueMessageRequest_GetValueSize(&buffer, &value_size);

    assert(size == buffer.buffer_size);
    assert(key_size_offset == MESSAGE_DATA_OFFSET);
    assert(key_size == 8);
    assert(value_size_offset == (MESSAGE_DATA_OFFSET + MESSAGE_SIZE_FIELD_BYTES + key_size));
    assert(value_size == 10);

    PutKeyValueMessageRequest_DestroyBuffer(&buffer);
}

static void testSerializeDeserialize()
{
    PutKeyValueMessageRequest req = {
        .key = {
            .raw_buffer = (uint8_t*)"test-key",
            .buffer_size = 8
        },
        .value = {
            .raw_buffer = (uint8_t*)"test-value",
            .buffer_size = 10
        }
    };

    SizeAwareBuffer serialized_message;
    PutKeyValueMessageRequest_AllocateBuffer(&req, &serialized_message);
    PutKeyValueMessageRequest_SerializeIntoBuffer(&req, &serialized_message);

    PutKeyValueMessageRequest result;
    PutKeyValueMessageRequest_AllocateMessage(&serialized_message, &result);
    PutKeyValueMessageRequest_Deserialize(&serialized_message, &result);

    assert(SizeAwareBuffer_AreContentsSame(&req.key, &result.key));
    assert(SizeAwareBuffer_AreContentsSame(&req.value, &result.value));

    PutKeyValueMessageRequest_DestroyBuffer(&serialized_message);
    PutKeyValueMessageRequest_DestroyMessage(&result);
}

int main()
{
    printf("STARTING PUT KEY VALUE MESSAGE REQUEST TEST\n");

    testSize();

    testSerailizedOffsets();

    testSerializeDeserialize();
}
