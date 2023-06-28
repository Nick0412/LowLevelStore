#include "messages/CommonUtils.h"
#include "messages/Constants.h"
#include "messages/MessageTypes.h"
#include <assert.h>
#include <stdio.h>

void testGetSizeFromMessageBuffer()
{
    uint32_t init_size = 123456;
    char temp_data[10];
    AugmentedBuffer test_buffer = {
        .buffer_pointer = temp_data,
        .buffer_size = 10
    };

    place32BitUintInBuffer(init_size, &test_buffer, MESSAGE_SIZE_OFFSET);
    uint32_t actual_size;
    Messages_GetSizeFromBuffer(&test_buffer, &actual_size);

    assert(actual_size == init_size);
}

void testGetTypeFromMessageBuffer()
{
    MessageType message_type = GET_VALUE_MESSAGE_REQUEST_TYPE;
    char temp_data[10];
    AugmentedBuffer test_buffer = {
        .buffer_pointer = temp_data,
        .buffer_size = 10
    };

    place32BitUintInBuffer((uint32_t)message_type, &test_buffer, MESSAGE_TYPE_OFFSET);
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
