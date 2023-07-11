#include "messages/PutKeyValueMessageRequest.h"
#include "messages/Constants.h"
#include <stdlib.h>
#include <stdio.h>

void PutKeyValueMessageRequest_CalculateSize(const PutKeyValueMessageRequest* request, uint32_t* return_request_size)
{
    uint32_t size = 0;
    
    size += MESSAGE_SIZE_BYTE_SIZE;
    size += MESSAGE_TYPE_BYTE_SIZE;
    size += MESSAGE_SIZE_FIELD_BYTES;
    size += request->key.buffer_size;
    size += MESSAGE_SIZE_FIELD_BYTES;
    size += request->value.buffer_size;

    *return_request_size = size;
}

void PutKeyValueMessageRequest_SerializeIntoBuffer(const PutKeyValueMessageRequest* request, SizeAwareBuffer* return_message_bytes)
{
    uint32_t offset = 0;
    uint32_t message_size;
    PutKeyValueMessageRequest_CalculateSize(request, &message_size);
    SizeAwareBuffer_Place32BitValue(message_size, return_message_bytes, offset);

    offset += MESSAGE_SIZE_BYTE_SIZE;
    MessageType message_type = PUT_KEY_VALUE_MESSAGE_REQUEST_TYPE;
    SizeAwareBuffer_Place32BitValue((uint32_t)message_type, return_message_bytes, offset);

    offset += MESSAGE_TYPE_BYTE_SIZE;
    SizeAwareBuffer_Place32BitValue(request->key.buffer_size, return_message_bytes, offset);

    offset += MESSAGE_SIZE_FIELD_BYTES;
    SizeAwareBuffer_PlaceStringInBuffer(&request->key, return_message_bytes, offset);

    offset += request->key.buffer_size;
    SizeAwareBuffer_Place32BitValue(request->value.buffer_size, return_message_bytes, offset);

    offset += MESSAGE_SIZE_FIELD_BYTES;
    SizeAwareBuffer_PlaceStringInBuffer(&request->value, return_message_bytes, offset);
}

void PutKeyValueMessageRequest_Deserialize(const SizeAwareBuffer* message_bytes, PutKeyValueMessageRequest* return_request)
{
    uint32_t key_offset;
    PutKeyValueMessageRequest_GetKeyOffset(message_bytes, &key_offset);

    uint32_t value_offset;
    PutKeyValueMessageRequest_GetValueOffset(message_bytes, &value_offset);

    SizeAwareBuffer_GetStringFromBuffer(message_bytes, key_offset, &return_request->key);
    SizeAwareBuffer_GetStringFromBuffer(message_bytes, value_offset, &return_request->value);
}

void PutKeyValueMessageRequest_AllocateBuffer(const PutKeyValueMessageRequest* request, SizeAwareBuffer* return_message_bytes)
{
    uint32_t size;
    PutKeyValueMessageRequest_CalculateSize(request, &size);

    SizeAwareBuffer_AllocateBuffer(size, return_message_bytes);
}

void PutKeyValueMessageRequest_DestroyBuffer(SizeAwareBuffer* message_bytes)
{
    SizeAwareBuffer_DestroyBuffer(message_bytes);
}

void PutKeyValueMessageRequest_AllocateMessage(const SizeAwareBuffer* message_bytes, PutKeyValueMessageRequest* return_request)
{
    uint32_t key_size;
    PutKeyValueMessageRequest_GetKeySize(message_bytes, &key_size);

    uint32_t value_size;
    PutKeyValueMessageRequest_GetValueSize(message_bytes, &value_size);

    SizeAwareBuffer_AllocateBuffer(key_size, &return_request->key);
    SizeAwareBuffer_AllocateBuffer(value_size, &return_request->value);
}

void PutKeyValueMessageRequest_DestroyMessage(PutKeyValueMessageRequest* request)
{
    SizeAwareBuffer_DestroyBuffer(&request->key);
    SizeAwareBuffer_DestroyBuffer(&request->value);
}

void PutKeyValueMessageRequest_GetKeySizeOffset(const SizeAwareBuffer* message_bytes, uint32_t* return_key_size_offset)
{
    (void)message_bytes;

    *return_key_size_offset = MESSAGE_DATA_OFFSET;
}

void PutKeyValueMessageRequest_GetKeySize(const SizeAwareBuffer* message_bytes, uint32_t* return_key_size)
{
    uint32_t offset;
    PutKeyValueMessageRequest_GetKeySizeOffset(message_bytes, &offset);

    SizeAwareBuffer_Get32BitValue(message_bytes, offset, return_key_size);
}

void PutKeyValueMessageRequest_GetKeyOffset(const SizeAwareBuffer* message_bytes, uint32_t* return_key_offset)
{
    uint32_t key_size_offset;
    PutKeyValueMessageRequest_GetKeySizeOffset(message_bytes, &key_size_offset);

    *return_key_offset = key_size_offset + MESSAGE_SIZE_FIELD_BYTES;
}

void PutKeyValueMessageRequest_GetKey(const SizeAwareBuffer* message_bytes, SizeAwareBuffer* return_key)
{
    uint32_t key_offset;
    PutKeyValueMessageRequest_GetKeyOffset(message_bytes, &key_offset);

    SizeAwareBuffer_GetStringFromBuffer(message_bytes, key_offset, return_key);
}

void PutKeyValueMessageRequest_GetValueSizeOffset(const SizeAwareBuffer* message_bytes, uint32_t* return_value_size_offset)
{
    uint32_t key_offset;
    PutKeyValueMessageRequest_GetKeyOffset(message_bytes, &key_offset);

    uint32_t key_size;
    PutKeyValueMessageRequest_GetKeySize(message_bytes, &key_size);

    *return_value_size_offset = key_offset + key_size;
}

void PutKeyValueMessageRequest_GetValueSize(const SizeAwareBuffer* message_bytes, uint32_t* return_value_size)
{
    uint32_t value_size_offset;
    PutKeyValueMessageRequest_GetValueSizeOffset(message_bytes, &value_size_offset);

    SizeAwareBuffer_Get32BitValue(message_bytes, value_size_offset, return_value_size);
}

void PutKeyValueMessageRequest_GetValueOffset(const SizeAwareBuffer* message_bytes, uint32_t* return_value_offset)
{
    uint32_t value_size_offset;
    PutKeyValueMessageRequest_GetValueSizeOffset(message_bytes, &value_size_offset);

    *return_value_offset = value_size_offset + MESSAGE_SIZE_FIELD_BYTES;
}

void PutKeyValueMessageRequest_GetValue(const SizeAwareBuffer* message_bytes, SizeAwareBuffer* return_value)
{
    uint32_t value_offset;
    PutKeyValueMessageRequest_GetValueOffset(message_bytes, &value_offset);

    SizeAwareBuffer_GetStringFromBuffer(message_bytes, value_offset, return_value);
}