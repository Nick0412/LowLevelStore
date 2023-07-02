#include "messages/GetValueMessageRequest.h"
#include "messages/MessageTypes.h"
#include "messages/Constants.h"

#include <stdio.h>
#include <stdlib.h>

void GetValueMessageRequest_GetKeySizeOffset(const SizeAwareBuffer* message_bytes, uint32_t* return_key_size_offset)
{
    *return_key_size_offset = MESSAGE_DATA_OFFSET;
}

void GetValueMessageRequest_GetKeySize(const SizeAwareBuffer* message_bytes, uint32_t* return_key_size)
{
    uint32_t key_size_offset;
    GetValueMessageRequest_GetKeySizeOffset(message_bytes, &key_size_offset);

    SizeAwareBuffer_Get32BitValue(message_bytes, key_size_offset, return_key_size);
}

void GetValueMessageRequest_GetKeyOffset(const SizeAwareBuffer* message_bytes, uint32_t* return_key_offset)
{
    uint32_t key_size_offset;
    GetValueMessageRequest_GetKeySizeOffset(message_bytes, &key_size_offset);

    *return_key_offset = key_size_offset + MESSAGE_SIZE_FIELD_BYTES;
}

void GetValueMessageRequest_GetKey(const SizeAwareBuffer* message_bytes, SizeAwareBuffer* return_key)
{
    uint32_t key_offset;
    GetValueMessageRequest_GetKeyOffset(message_bytes, &key_offset);

    SizeAwareBuffer_GetStringFromBuffer(message_bytes, key_offset, return_key);
}

void GetValueMessageRequest_CalculateSize(const GetValueMessageRequest* request, uint32_t* return_request_size)
{
    uint32_t message_size = 0;
    message_size += MESSAGE_SIZE_BYTE_SIZE;
    message_size += MESSAGE_TYPE_BYTE_SIZE;
    message_size += MESSAGE_SIZE_FIELD_BYTES;
    message_size += request->key.buffer_size;

    *return_request_size = message_size;
}

void GetValueMessageRequest_AllocateBuffer(const GetValueMessageRequest* request, SizeAwareBuffer* return_message_bytes)
{
    uint32_t message_size;
    GetValueMessageRequest_CalculateSize(request, &message_size);

    SizeAwareBuffer_AllocateBuffer(message_size, return_message_bytes);
}

void GetValueMessageRequest_DestroyBuffer(SizeAwareBuffer* message_bytes)
{
    SizeAwareBuffer_DestroyBuffer(message_bytes);
}

void GetValueMessageRequest_AllocateMessage(const SizeAwareBuffer* message_bytes, GetValueMessageRequest* return_request)
{
    uint32_t key_size;
    GetValueMessageRequest_GetKeySize(message_bytes, &key_size);

    SizeAwareBuffer_AllocateBuffer(key_size, &return_request->key);
}

void GetValueMessageRequest_DestroyMessage(GetValueMessageRequest* request)
{
    SizeAwareBuffer_DestroyBuffer(&request->key);
}

void GetValueMessageRequest_SerializeIntoBuffer(const GetValueMessageRequest* request, SizeAwareBuffer* return_message_bytes)
{
    // Place message size
    uint32_t offset = 0;
    uint32_t message_size;
    GetValueMessageRequest_CalculateSize(request, &message_size);
    SizeAwareBuffer_Place32BitValue(message_size, return_message_bytes, offset);

    // Place message type
    offset += MESSAGE_SIZE_BYTE_SIZE;
    MessageType message_type = GET_VALUE_MESSAGE_REQUEST_TYPE;
    SizeAwareBuffer_Place32BitValue((uint32_t)message_type, return_message_bytes, offset);

    // Place key size
    offset += MESSAGE_TYPE_BYTE_SIZE;
    SizeAwareBuffer_Place32BitValue(request->key.buffer_size, return_message_bytes, offset);

    // Place key data
    offset += MESSAGE_SIZE_FIELD_BYTES;
    SizeAwareBuffer_PlaceStringInBuffer(&request->key, return_message_bytes, offset);
}

void GetValueMessageRequest_Deserialize(const SizeAwareBuffer* message_bytes, GetValueMessageRequest* return_request)
{
    uint32_t key_offset;
    GetValueMessageRequest_GetKeyOffset(message_bytes, &key_offset);

    SizeAwareBuffer_GetStringFromBuffer(message_bytes, key_offset, &return_request->key);
}
