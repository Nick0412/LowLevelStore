#include "messages/GetValueMessageResponse.h"
#include "messages/Common.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

void testCalculateGetValueMessageRequestSize()
{
    printf("  - testCalculateGetValueMessageRequestSize\n");

    GetValueMessageResponseSuccess res = {
        .value = {
            .raw_buffer = (uint8_t*)"testvalue",
            .buffer_size = 9
        }
    };

    uint32_t response_size;
    GetValueMessageResponseSuccess_CalculateSize(&res, &response_size);

    assert(response_size == (
        MESSAGE_SIZE_BYTE_SIZE + MESSAGE_TYPE_BYTE_SIZE +
        MESSAGE_SIZE_FIELD_BYTES + res.value.buffer_size));
}

void testSerializeDeserialize()
{
    printf("  - testSerializeDeserialize\n");

    GetValueMessageResponseSuccess res = {
        .value = {
            .raw_buffer = (uint8_t*)"testvalue",
            .buffer_size = 9
        }
    };

    SizeAwareBuffer buffer;
    GetValueMessageResponseSuccess_AllocateBuffer(&res, &buffer);
    GetValueMessageResponseSuccess_SerializeIntoBuffer(&res, &buffer);

    GetValueMessageResponseSuccess decoded_res;
    GetValueMessageResponseSuccess_AllocateMessage(&buffer, &decoded_res);
    GetValueMessageResponseSuccess_Deserialize(&buffer, &decoded_res);

    assert(SizeAwareBuffer_AreContentsSame(&res.value, &decoded_res.value));

    GetValueMessageResponseSuccess_DestroyBuffer(&buffer);
    GetValueMessageResponseSuccess_DestroyMessage(&decoded_res);
}

int main()
{
    printf("STARTING GET VALUE MESSAGE RESPONSE TEST\n");

    testCalculateGetValueMessageRequestSize();

    testSerializeDeserialize();
}
