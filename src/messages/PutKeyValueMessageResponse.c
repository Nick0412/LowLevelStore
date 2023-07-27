#include "messages/PutKeyValueMessageResponse.h"
#include "messages/Common.h"

void PutKeyValueMessageResponse_CalculateSize(const PutKeyValueMessageResponse* response, uint32_t* return_response_size)
{
    (void)response;

    uint32_t size = 0;

    size += MESSAGE_SIZE_BYTE_SIZE;
    size += MESSAGE_TYPE_BYTE_SIZE;
    size += MESSAGE_SIZE_FIELD_BYTES;

    *return_response_size = size;
}

void PutKeyValueMessageResponse_SerializeIntoBuffer(const PutKeyValueMessageResponse* response, SizeAwareBuffer* return_message_bytes)
{
    uint32_t offset = 0;
    uint32_t message_size;
    PutKeyValueMessageResponse_CalculateSize(response, &message_size);
    SizeAwareBuffer_Place32BitValue(message_size, return_message_bytes, offset);

    offset += MESSAGE_SIZE_BYTE_SIZE;
    MessageType type = PUT_KEY_VALUE_MESSAGE_RESPONSE_TYPE;
    SizeAwareBuffer_Place32BitValue((uint32_t)type, return_message_bytes, offset);

    offset += MESSAGE_TYPE_BYTE_SIZE;
    SizeAwareBuffer_Place32BitValue((uint32_t)response->status, return_message_bytes, offset);
}

void PutKeyValueMessageResponse_Deserialize(const SizeAwareBuffer* message_bytes, PutKeyValueMessageResponse* return_response)
{
    MessageStatus status;
    PutKeyValueMessageResponse_GetStatus(message_bytes, &status);

    return_response->status = status;
}

void PutKeyValueMessageResponse_AllocateBuffer(const PutKeyValueMessageResponse* response, SizeAwareBuffer* return_message_bytes)
{
    uint32_t message_size;
    PutKeyValueMessageResponse_CalculateSize(response, &message_size);

    SizeAwareBuffer_AllocateBuffer(message_size, return_message_bytes);
}

void PutKeyValueMessageResponse_DestroyBuffer(SizeAwareBuffer* message_bytes)
{
    SizeAwareBuffer_DestroyBuffer(message_bytes);
}

void PutKeyValueMessageResponse_AllocateMessage(const SizeAwareBuffer* message_bytes, PutKeyValueMessageResponse* return_response)
{
    (void)message_bytes;
    (void)return_response;
}

void PutKeyValueMessageResponse_DestroyMessage(PutKeyValueMessageResponse* response)
{
    (void)response;
}

void PutKeyValueMessageResponse_GetStatusOffset(const SizeAwareBuffer* message_bytes, uint32_t* return_status_offset)
{
    (void)message_bytes;
    
    *return_status_offset = MESSAGE_DATA_OFFSET;
}

void PutKeyValueMessageResponse_GetStatus(const SizeAwareBuffer* message_bytes, MessageStatus* return_status)
{
    uint32_t status_offset;
    PutKeyValueMessageResponse_GetStatusOffset(message_bytes, &status_offset);

    uint32_t message_status_number;
    SizeAwareBuffer_Get32BitValue(message_bytes, status_offset, &message_status_number);

    *return_status = (MessageStatus)message_status_number;
}
