#include "messages/GetValueMessageResponse.h"
#include "messages/Common.h"
#include "common/Constants.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

void testCalculateGetValueMessageResponseSize()
{
    printf("  - testCalculateGetValueMessageResponseSize\n");

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

void testSuccessSerializeDeserialize()
{
    printf("  - testSuccessSerializeDeserialize\n");

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

void testErrorGetSize()
{
    printf("  - testErrorGetSize\n");

    char* message = "this is an error";
    GetValueMessageResponseError error = {
        .error_code = KEY_DOES_NOT_EXIST,
        .error_details = {
            .buffer_size = strlen(message),
            .raw_buffer = (uint8_t*)message
        }
    };

    uint32_t message_size;
    GetValueMessageResponseError_CalculateSize(&error, &message_size);

    assert(message_size == (MESSAGE_SIZE_BYTE_SIZE + 
                            MESSAGE_TYPE_BYTE_SIZE +
                            CONSTANT_UINT_32_SIZE +
                            CONSTANT_UINT_32_SIZE +
                            strlen(message)));
}

void testErrorSerializeDeserialize()
{
    printf("  - testErrorSerializeDeserialize\n");

    char* message = "this is an error";
    GetValueMessageResponseError error = {
        .error_code = KEY_DOES_NOT_EXIST,
        .error_details = {
            .buffer_size = strlen(message),
            .raw_buffer = (uint8_t*)message
        }
    };

    SizeAwareBuffer buffer;
    GetValueMessageResponseError_AllocateBuffer(&error, &buffer);
    GetValueMessageResponseError_SerializeIntoBuffer(&error, &buffer);

    GetValueMessageResponseError result;
    GetValueMessageResponseError_AllocateMessage(&buffer, &result);
    GetValueMessageResponseError_Deserialize(&buffer, &result);

    assert(result.error_code == KEY_DOES_NOT_EXIST);
    assert(SizeAwareBuffer_AreContentsSame(&result.error_details, &error.error_details));

    GetValueMessageResponseError_DestroyBuffer(&buffer);
    GetValueMessageResponseError_DestroyMessage(&result);
}

int main()
{
    printf("STARTING GET VALUE MESSAGE RESPONSE TEST\n");

    testCalculateGetValueMessageResponseSize();

    testSuccessSerializeDeserialize();

    testErrorGetSize();

    testErrorSerializeDeserialize();
}
