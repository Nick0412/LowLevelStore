#include "messages/GetValueMessageResponse.h"
#include "messages/Common.h"

void GetValueMessageResponse_CalculateSize(const GetValueMessageResponse* response, uint32_t* return_size)
{
    uint32_t size = 0;

    size += MESSAGE_SIZE_BYTE_SIZE;
    size += MESSAGE_TYPE_BYTE_SIZE;
    size += MESSAGE_SIZE_FIELD_BYTES;
    size += response->value.buffer_size;

    *return_size = size;
}

void GetValueMessageResponse_AllocateBuffer(const GetValueMessageResponse* response, SizeAwareBuffer* return_message_bytes)
{
    uint32_t size;
    GetValueMessageResponse_CalculateSize(response, &size);

    SizeAwareBuffer_AllocateBuffer(size, return_message_bytes);
}

void GetValueMessageResponse_DestroyBuffer(SizeAwareBuffer* message_bytes)
{
    SizeAwareBuffer_DestroyBuffer(message_bytes);
}

void GetValueMessageResponse_GetValueSizeOffset(const SizeAwareBuffer* message_bytes, uint32_t* return_value_size_offset)
{
    (void)message_bytes;

    *return_value_size_offset = MESSAGE_DATA_OFFSET;
}

void GetValueMessageResponse_GetValueSize(const SizeAwareBuffer* message_bytes, uint32_t* return_value_size)
{
    uint32_t offset;
    GetValueMessageResponse_GetValueSizeOffset(message_bytes, &offset);

    SizeAwareBuffer_Get32BitValue(message_bytes, offset, return_value_size);
}

void GetValueMessageResponse_GetValueOffset(const SizeAwareBuffer* message_bytes, uint32_t* return_value_offset)
{
    uint32_t value_size_offset;
    GetValueMessageResponse_GetValueSizeOffset(message_bytes, &value_size_offset);

    *return_value_offset = value_size_offset + MESSAGE_SIZE_FIELD_BYTES;
}

void GetValueMessageResponse_GetValue(const SizeAwareBuffer* message_bytes, SizeAwareBuffer* return_value)
{
    uint32_t value_offset;
    GetValueMessageResponse_GetValueOffset(message_bytes, &value_offset);

    SizeAwareBuffer_GetStringFromBuffer(message_bytes, value_offset, return_value);
}

void GetValueMessageResponse_SerializeIntoBuffer(const GetValueMessageResponse* response, SizeAwareBuffer* return_message_bytes)
{
    uint32_t offset = 0;
    uint32_t message_size;
    GetValueMessageResponse_CalculateSize(response, &message_size);
    SizeAwareBuffer_Place32BitValue(message_size, return_message_bytes, offset);

    offset += MESSAGE_SIZE_BYTE_SIZE;
    MessageType message_type = GET_VALUE_MESSAGE_RESPONSE_TYPE;
    SizeAwareBuffer_Place32BitValue((uint32_t)message_type, return_message_bytes, offset);

    offset += MESSAGE_TYPE_BYTE_SIZE;
    SizeAwareBuffer_Place32BitValue(response->value.buffer_size, return_message_bytes, offset);

    offset += MESSAGE_SIZE_FIELD_BYTES;
    SizeAwareBuffer_PlaceStringInBuffer(&response->value, return_message_bytes, offset);
}

void GetValueMessageResponse_Deserialize(const SizeAwareBuffer* message_bytes, GetValueMessageResponse* return_response)
{
    uint32_t value_offset;
    GetValueMessageResponse_GetValueOffset(message_bytes, &value_offset);

    SizeAwareBuffer_GetStringFromBuffer(message_bytes, value_offset, &return_response->value);
}

void GetValueMessageResponse_AllocateMessage(const SizeAwareBuffer* message_bytes, GetValueMessageResponse* return_response)
{
    uint32_t value_size;
    GetValueMessageResponse_GetValueSize(message_bytes, &value_size);

    SizeAwareBuffer_AllocateBuffer(value_size, &return_response->value);
}

void GetValueMessageResponse_DestroyMessage(GetValueMessageResponse* response)
{
    SizeAwareBuffer_DestroyBuffer(&response->value);
}
