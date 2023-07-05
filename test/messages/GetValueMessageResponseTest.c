#include "messages/GetValueMessageResponse.h"
#include "messages/Constants.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

void testCalculateGetValueMessageRequestSize()
{
    printf("  - testCalculateGetValueMessageRequestSize\n");

    GetValueMessageResponse res = {
        .value = {
            .raw_buffer = (uint8_t*)"testvalue",
            .buffer_size = 9
        }
    };

    uint32_t response_size;
    GetValueMessageResponse_CalculateSize(&res, &response_size);

    assert(response_size == (
        MESSAGE_SIZE_BYTE_SIZE + MESSAGE_TYPE_BYTE_SIZE +
        MESSAGE_SIZE_FIELD_BYTES + res.value.buffer_size));
}

void testSerializeDeserialize()
{
    printf("  - testSerializeDeserialize\n");

    GetValueMessageResponse res = {
        .value = {
            .raw_buffer = (uint8_t*)"testvalue",
            .buffer_size = 9
        }
    };

    SizeAwareBuffer buffer;
    GetValueMessageResponse_AllocateBuffer(&res, &buffer);
    GetValueMessageResponse_SerializeIntoBuffer(&res, &buffer);

    GetValueMessageResponse decoded_res;
    GetValueMessageResponse_AllocateMessage(&buffer, &decoded_res);
    GetValueMessageResponse_Deserialize(&buffer, &decoded_res);

    assert(SizeAwareBuffer_AreContentsSame(&res.value, &decoded_res.value));

    GetValueMessageResponse_DestroyBuffer(&buffer);
    GetValueMessageResponse_DestroyMessage(&decoded_res);
}

int main()
{
    printf("STARTING GET VALUE MESSAGE RESPONSE TEST\n");

    testCalculateGetValueMessageRequestSize();

    testSerializeDeserialize();
}
