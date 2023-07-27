#include "messages/GetValueMessageRequest.h"
#include "messages/Common.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

void testCalculateGetValueMessageRequestSize()
{
    printf("  - testCalculateGetValueMessageRequestSize\n");

    GetValueMessageRequest message = {
        .key = {
            .raw_buffer = (uint8_t*)"test_key",
            .buffer_size = 8
        }
    };

    uint32_t size;
    GetValueMessageRequest_CalculateSize(&message, &size);

    assert(size == (
        MESSAGE_SIZE_BYTE_SIZE + MESSAGE_TYPE_BYTE_SIZE +
        MESSAGE_SIZE_FIELD_BYTES + message.key.buffer_size));
}

void testSerializeDeserialize()
{
    printf("  - testSerializeDeserialize\n");

    GetValueMessageRequest req = {
        .key = {
            .raw_buffer = (uint8_t*)"this-test-key",
            .buffer_size = 13
        }
    };

    uint32_t message_size;
    GetValueMessageRequest_CalculateSize(&req, &message_size);

    SizeAwareBuffer buffer;
    GetValueMessageRequest_AllocateBuffer(&req, &buffer);
    GetValueMessageRequest_SerializeIntoBuffer(&req, &buffer);

    GetValueMessageRequest output_req;
    GetValueMessageRequest_AllocateMessage(&buffer, &output_req);
    GetValueMessageRequest_Deserialize(&buffer, &output_req);

    assert(SizeAwareBuffer_AreContentsSame(&req.key, &output_req.key));

    GetValueMessageRequest_DestroyBuffer(&buffer);
    GetValueMessageRequest_DestroyMessage(&output_req);
}

int main()
{
    printf("STARTING GET VALUE MESSAGE REQUEST TEST\n");

    testCalculateGetValueMessageRequestSize();

    testSerializeDeserialize();
}
