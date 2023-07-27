#include "messages/Common.h"
#include "messages/MessageTypes.h"
#include <assert.h>
#include <stdio.h>

void testGetSizeFromMessageBuffer()
{
    printf("  - testGetSizeFromMessageBuffer\n");

    uint32_t init_size = 123456;
    char temp_data[10];
    SizeAwareBuffer test_buffer = {
        .raw_buffer = (uint8_t*)temp_data,
        .buffer_size = 10
    };

    SizeAwareBuffer_Place32BitValue(init_size, &test_buffer, MESSAGE_SIZE_OFFSET);

    uint32_t actual_size;
    Messages_GetSizeFromBuffer(&test_buffer, &actual_size);

    assert(actual_size == init_size);
}

void testGetTypeFromMessageBuffer()
{
    printf("  - testGetTypeFromMessageBuffer\n");

    MessageType message_type = GET_VALUE_MESSAGE_REQUEST_TYPE;
    uint8_t temp_data[10] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a' };
    SizeAwareBuffer test_buffer = {
        .raw_buffer = temp_data,
        .buffer_size = 10
    };

    SizeAwareBuffer_Place32BitValue((uint32_t)message_type, &test_buffer, MESSAGE_TYPE_OFFSET);

    MessageType actual_type;
    Messages_GetTypeFromBuffer(&test_buffer, &actual_type);

    assert(actual_type == message_type);
}

int main()
{
    printf("STARTING MESSAGES COMMON UTILS TEST\n");
    
    testGetSizeFromMessageBuffer();

    testGetTypeFromMessageBuffer();

    return 0;
}
