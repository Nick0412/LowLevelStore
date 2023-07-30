#include "messages/GetValueMessageResponse.h"
#include "messages/Common.h"
#include "common/Constants.h"

void GetValueMessageResponseSuccess_CalculateSize(const GetValueMessageResponseSuccess* response, uint32_t* return_size)
{
    uint32_t size = 0;

    size += MESSAGE_SIZE_BYTE_SIZE;
    size += MESSAGE_TYPE_BYTE_SIZE;
    size += MESSAGE_SIZE_FIELD_BYTES;
    size += response->value.buffer_size;

    *return_size = size;
}

void GetValueMessageResponseSuccess_AllocateBuffer(
    const GetValueMessageResponseSuccess* response,
    SizeAwareBuffer* return_message_bytes)
{
    uint32_t size;
    GetValueMessageResponseSuccess_CalculateSize(response, &size);

    SizeAwareBuffer_AllocateBuffer(size, return_message_bytes);
}

void GetValueMessageResponseSuccess_DestroyBuffer(SizeAwareBuffer* message_bytes)
{
    SizeAwareBuffer_DestroyBuffer(message_bytes);
}

void GetValueMessageResponseSuccess_GetValueSizeOffset(
    const SizeAwareBuffer* message_bytes,
    uint32_t* return_value_size_offset)
{
    (void)message_bytes;

    *return_value_size_offset = MESSAGE_DATA_OFFSET;
}

void GetValueMessageResponseSuccess_GetValueSize(
    const SizeAwareBuffer* message_bytes,
    uint32_t* return_value_size)
{
    uint32_t offset;
    GetValueMessageResponseSuccess_GetValueSizeOffset(message_bytes, &offset);

    SizeAwareBuffer_Get32BitValue(message_bytes, offset, return_value_size);
}

void GetValueMessageResponseSuccess_GetValueOffset(
    const SizeAwareBuffer* message_bytes,
    uint32_t* return_value_offset)
{
    uint32_t value_size_offset;
    GetValueMessageResponseSuccess_GetValueSizeOffset(message_bytes, &value_size_offset);

    *return_value_offset = value_size_offset + MESSAGE_SIZE_FIELD_BYTES;
}

void GetValueMessageResponseSuccess_GetValue(
    const SizeAwareBuffer* message_bytes,
    SizeAwareBuffer* return_value)
{
    uint32_t value_offset;
    GetValueMessageResponseSuccess_GetValueOffset(message_bytes, &value_offset);

    SizeAwareBuffer_GetStringFromBuffer(message_bytes, value_offset, return_value);
}

void GetValueMessageResponseSuccess_SerializeIntoBuffer(
    const GetValueMessageResponseSuccess* response,
    SizeAwareBuffer* return_message_bytes)
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

void GetValueMessageResponseSuccess_Deserialize(
    const SizeAwareBuffer* message_bytes,
    GetValueMessageResponseSuccess* return_response)
{
    uint32_t value_offset;
    GetValueMessageResponseSuccess_GetValueOffset(message_bytes, &value_offset);

    SizeAwareBuffer_GetStringFromBuffer(message_bytes, value_offset, &return_response->value);
}

void GetValueMessageResponseSuccess_AllocateMessage(
    const SizeAwareBuffer* message_bytes,
    GetValueMessageResponseSuccess* return_response)
{
    uint32_t value_size;
    GetValueMessageResponseSuccess_GetValueSize(message_bytes, &value_size);

    SizeAwareBuffer_AllocateBuffer(value_size, &return_response->value);
}

void GetValueMessageResponseSuccess_DestroyMessage(GetValueMessageResponseSuccess* response)
{
    SizeAwareBuffer_DestroyBuffer(&response->value);
}

void GetValueMessageResponseError_CalculateSize(
    const GetValueMessageResponseError* response,
    uint32_t* return_size)
{
    uint32_t size = 0;

    size += MESSAGE_SIZE_BYTE_SIZE;
    size += MESSAGE_TYPE_BYTE_SIZE;
    size += CONSTANT_UINT_32_SIZE;
    size += CONSTANT_UINT_32_SIZE;
    size += response->error_details.buffer_size;

    *return_size = size;
}

void GetValueMessageResponseError_SerializeIntoBuffer(
    const GetValueMessageResponseError* response,
    SizeAwareBuffer* return_message_bytes)
{
    uint32_t offset = 0;
    uint32_t message_size;
    GetValueMessageResponseError_CalculateSize(response, &message_size);
    SizeAwareBuffer_Place32BitValue(message_size, return_message_bytes, offset);

    offset += MESSAGE_SIZE_BYTE_SIZE;
    SizeAwareBuffer_Place32BitValue((uint32_t)GET_VALUE_MESSAGE_ERROR_TYPE, return_message_bytes, offset);

    offset += MESSAGE_TYPE_BYTE_SIZE;
    SizeAwareBuffer_Place32BitValue((uint32_t)response->error_code, return_message_bytes, offset);

    offset += CONSTANT_UINT_32_SIZE;
    SizeAwareBuffer_Place32BitValue(response->error_details.buffer_size, return_message_bytes, offset);

    offset += CONSTANT_UINT_32_SIZE;
    SizeAwareBuffer_PlaceStringInBuffer(&response->error_details, return_message_bytes, offset);
}

void GetValueMessageResponseError_Deserialize(
    const SizeAwareBuffer* message_bytes,
    GetValueMessageResponseError* return_response)
{
    GetValueMessageResponseError_GetErrorCode(message_bytes, &return_response->error_code);
    GetValueMessageResponseError_GetErrorDetails(message_bytes, &return_response->error_details);
}

void GetValueMessageResponseError_AllocateBuffer(
    const GetValueMessageResponseError* response,
    SizeAwareBuffer* return_message_bytes)
{
    uint32_t size;
    GetValueMessageResponseError_CalculateSize(response, &size);

    SizeAwareBuffer_AllocateBuffer(size, return_message_bytes);
}

void GetValueMessageResponseError_DestroyBuffer(SizeAwareBuffer* message_bytes)
{
    SizeAwareBuffer_DestroyBuffer(message_bytes);
}

void GetValueMessageResponseError_AllocateMessage(
    const SizeAwareBuffer* message_bytes,
    GetValueMessageResponseError* return_response)
{
    uint32_t error_details_size;
    GetValueMessageResponseError_GetErrorDetailsSize(message_bytes, &error_details_size);

    SizeAwareBuffer_AllocateBuffer(error_details_size, &return_response->error_details);
}

void GetValueMessageResponseError_DestroyMessage(GetValueMessageResponseError* response)
{
    SizeAwareBuffer_DestroyBuffer(&response->error_details);
}

void GetValueMessageResponseError_GetErrorCodeOffset(
    const SizeAwareBuffer* message_bytes,
    uint32_t* return_code_offset)
{
    (void)message_bytes;

    *return_code_offset = MESSAGE_SIZE_BYTE_SIZE + MESSAGE_TYPE_BYTE_SIZE;
}

void GetValueMessageResponseError_GetErrorCode(
    const SizeAwareBuffer* message_bytes,
    GetValueMessageResponseErrorCode* return_error_code)
{
    uint32_t offset;
    GetValueMessageResponseError_GetErrorCodeOffset(message_bytes, &offset);

    uint32_t error_code_number;
    SizeAwareBuffer_Get32BitValue(message_bytes, offset, &error_code_number);

    *return_error_code = (GetValueMessageResponseErrorCode)error_code_number;
}

void GetValueMessageResponseError_GetErrorDetailsSizeOffset(
    const SizeAwareBuffer* message_bytes,
    uint32_t* return_details_size_offset)
{
    (void)message_bytes;

    *return_details_size_offset = MESSAGE_SIZE_BYTE_SIZE +
                                  MESSAGE_TYPE_BYTE_SIZE +
                                  CONSTANT_UINT_32_SIZE;
}

void GetValueMessageResponseError_GetErrorDetailsSize(
    const SizeAwareBuffer* message_bytes,
    uint32_t* return_details_size)
{
    uint32_t offset;
    GetValueMessageResponseError_GetErrorDetailsSizeOffset(message_bytes, &offset);

    SizeAwareBuffer_Get32BitValue(message_bytes, offset, return_details_size);
}

void GetValueMessageResponseError_GetErrorDetailsOffset(
    const SizeAwareBuffer* message_bytes,
    uint32_t* return_details_offset)
{
    (void)message_bytes;

    *return_details_offset = MESSAGE_SIZE_BYTE_SIZE +
                             MESSAGE_TYPE_BYTE_SIZE +
                             CONSTANT_UINT_32_SIZE +
                             CONSTANT_UINT_32_SIZE;
}

void GetValueMessageResponseError_GetErrorDetails(
    const SizeAwareBuffer* message_bytes,
    SizeAwareBuffer* return_error_details)
{
    uint32_t offset;
    GetValueMessageResponseError_GetErrorDetailsOffset(message_bytes, &offset);

    SizeAwareBuffer_GetStringFromBuffer(message_bytes, offset, return_error_details);
}
