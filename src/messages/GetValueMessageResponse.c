#include "messages/GetValueMessageResponse.h"
#include "messages/Common.h"

void GetValueMessageResponseSuccess_CalculateSize(const GetValueMessageResponseSuccess* response, uint32_t* return_size)
{
    uint32_t size = 0;

    size += MESSAGE_SIZE_BYTE_SIZE;
    size += MESSAGE_TYPE_BYTE_SIZE;
    size += MESSAGE_SIZE_FIELD_BYTES;
    size += response->value.buffer_size;

    *return_size = size;
}

void GetValueMessageResponseSuccess_AllocateBuffer(const GetValueMessageResponseSuccess* response, SizeAwareBuffer* return_message_bytes)
{
    uint32_t size;
    GetValueMessageResponseSuccess_CalculateSize(response, &size);

    SizeAwareBuffer_AllocateBuffer(size, return_message_bytes);
}

void GetValueMessageResponseSuccess_DestroyBuffer(SizeAwareBuffer* message_bytes)
{
    SizeAwareBuffer_DestroyBuffer(message_bytes);
}

void GetValueMessageResponseSuccess_GetValueSizeOffset(const SizeAwareBuffer* message_bytes, uint32_t* return_value_size_offset)
{
    (void)message_bytes;

    *return_value_size_offset = MESSAGE_DATA_OFFSET;
}

void GetValueMessageResponseSuccess_GetValueSize(const SizeAwareBuffer* message_bytes, uint32_t* return_value_size)
{
    uint32_t offset;
    GetValueMessageResponseSuccess_GetValueSizeOffset(message_bytes, &offset);

    SizeAwareBuffer_Get32BitValue(message_bytes, offset, return_value_size);
}

void GetValueMessageResponseSuccess_GetValueOffset(const SizeAwareBuffer* message_bytes, uint32_t* return_value_offset)
{
    uint32_t value_size_offset;
    GetValueMessageResponseSuccess_GetValueSizeOffset(message_bytes, &value_size_offset);

    *return_value_offset = value_size_offset + MESSAGE_SIZE_FIELD_BYTES;
}

void GetValueMessageResponseSuccess_GetValue(const SizeAwareBuffer* message_bytes, SizeAwareBuffer* return_value)
{
    uint32_t value_offset;
    GetValueMessageResponseSuccess_GetValueOffset(message_bytes, &value_offset);

    SizeAwareBuffer_GetStringFromBuffer(message_bytes, value_offset, return_value);
}

void GetValueMessageResponseSuccess_SerializeIntoBuffer(const GetValueMessageResponseSuccess* response, SizeAwareBuffer* return_message_bytes)
{
    uint32_t offset = 0;
    uint32_t message_size;
    GetValueMessageResponseSuccess_CalculateSize(response, &message_size);
    SizeAwareBuffer_Place32BitValue(message_size, return_message_bytes, offset);

    offset += MESSAGE_SIZE_BYTE_SIZE;
    MessageType message_type = GET_VALUE_MESSAGE_RESPONSE_TYPE;
    SizeAwareBuffer_Place32BitValue((uint32_t)message_type, return_message_bytes, offset);

    offset += MESSAGE_TYPE_BYTE_SIZE;
    SizeAwareBuffer_Place32BitValue(response->value.buffer_size, return_message_bytes, offset);

    offset += MESSAGE_SIZE_FIELD_BYTES;
    SizeAwareBuffer_PlaceStringInBuffer(&response->value, return_message_bytes, offset);
}

void GetValueMessageResponseSuccess_Deserialize(const SizeAwareBuffer* message_bytes, GetValueMessageResponseSuccess* return_response)
{
    uint32_t value_offset;
    GetValueMessageResponseSuccess_GetValueOffset(message_bytes, &value_offset);

    SizeAwareBuffer_GetStringFromBuffer(message_bytes, value_offset, &return_response->value);
}

void GetValueMessageResponseSuccess_AllocateMessage(const SizeAwareBuffer* message_bytes, GetValueMessageResponseSuccess* return_response)
{
    uint32_t value_size;
    GetValueMessageResponseSuccess_GetValueSize(message_bytes, &value_size);

    SizeAwareBuffer_AllocateBuffer(value_size, &return_response->value);
}

void GetValueMessageResponseSuccess_DestroyMessage(GetValueMessageResponseSuccess* response)
{
    SizeAwareBuffer_DestroyBuffer(&response->value);
}
