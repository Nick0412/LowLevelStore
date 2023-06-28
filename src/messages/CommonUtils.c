#include "messages/CommonUtils.h"
#include "messages/Constants.h"
#include <stdio.h>

void Messages_GetSizeFromBuffer(const AugmentedBuffer* message_bytes, uint32_t* return_message_bytes_size)
{
    if (message_bytes->buffer_size < 4)
    {
        perror("The message buffer is not large enough to hold a size.\n");
    }

    get32BitUintFromBuffer(message_bytes, MESSAGE_SIZE_OFFSET, return_message_bytes_size);
}

void Messages_GetTypeFromBuffer(const AugmentedBuffer* message_bytes, MessageType* return_message_type)
{
    if (message_bytes->buffer_size < 8)
    {
        perror("The message buffer is not large enough to hold the message type.\n");
    }

    uint32_t message_type_number;
    get32BitUintFromBuffer(message_bytes, MESSAGE_TYPE_OFFSET, &message_type_number);

    *return_message_type = (MessageType)message_type_number;
}
