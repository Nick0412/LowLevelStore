#include "client-server/ServerHandler.h"
#include "common/TcpFunctions.h"
#include "messages/Common.h"
#include "messages/GetValueMessageResponse.h"
#include "messages/PutKeyValueMessageResponse.h"
#include <stdio.h>

void ServerHandler_HandleMessage(int socket, InMemoryDataStore* store)
{
    uint32_t message_size;
    TcpFunction_ReceiveBufferSizeOverSocket(socket, &message_size);

    SizeAwareBuffer incoming_message;
    SizeAwareBuffer_AllocateBuffer(message_size, &incoming_message);
    SizeAwareBuffer_Place32BitValue(message_size, &incoming_message, MESSAGE_SIZE_OFFSET);
    TcpFunction_ReceiveBufferOverSocketWithRetry(socket,
        &incoming_message,
        MESSAGE_TYPE_OFFSET,
        MESSAGE_SIZE_BYTE_SIZE);

    MessageType message_type;
    Messages_GetTypeFromBuffer(&incoming_message, &message_type);

    switch (message_type)
    {
        case GET_VALUE_MESSAGE_REQUEST_TYPE:
        {
            printf("In Get Value Message Handler\n");
            GetValueMessageRequest request;
            GetValueMessageRequest_AllocateMessage(&incoming_message, &request);
            GetValueMessageRequest_Deserialize(&incoming_message, &request);

            ServerHandler_HandleGetValueMessage(socket, store, &request);
            GetValueMessageRequest_DestroyMessage(&request);
            break;
        }
        
        case PUT_KEY_VALUE_MESSAGE_REQUEST_TYPE:
        {
            printf("In Put Key Value Message Handler\n");
            PutKeyValueMessageRequest request;
            PutKeyValueMessageRequest_AllocateMessage(&incoming_message, &request);
            PutKeyValueMessageRequest_Deserialize(&incoming_message, &request);

            ServerHandler_HandlePutKeyValueMessage(socket, store, &request);
            PutKeyValueMessageRequest_DestroyMessage(&request);
            break;
        }

        default:
            break;
    }

    SizeAwareBuffer_DestroyBuffer(&incoming_message);
}

void ServerHandler_HandleGetValueMessage(int socket, InMemoryDataStore* store,
    const GetValueMessageRequest* request)
{
    GetValueMessageResponse response;
    SizeAwareBuffer* pointer_to_value;
    SizeAwareBuffer return_bytes;
    Status status = InMemoryDataStore_FindValue(store, &request->key, &pointer_to_value);

    if (Status_IsSuccessful(&status))
    {
        response.value = *pointer_to_value;
        GetValueMessageResponse_AllocateBuffer(&response, &return_bytes);
        GetValueMessageResponse_SerializeIntoBuffer(&response, &return_bytes);
        TcpFunction_SendBufferOverSocketWithRetry(socket, &return_bytes);
        GetValueMessageResponse_DestroyBuffer(&return_bytes);
    }
    // TODO: Fill out failing case implementations later
    else
    {
        SizeAwareBuffer_PrintBuffer(Status_GetError(&status));
        Status_CleanUpResources(&status);
    }

}

void ServerHandler_HandlePutKeyValueMessage(int socket, InMemoryDataStore* store,
    const PutKeyValueMessageRequest* request)
{
    PutKeyValueMessageResponse response;
    SizeAwareBuffer return_bytes;
    KeyValueEntity kv = { .key = request->key, .value = request->value };
    Status status = InMemoryDataStore_InsertKeyValuePair(store, &kv);

    if (Status_IsSuccessful(&status))
    {
        response.status = MESSAGE_SUCCESS;
        PutKeyValueMessageResponse_AllocateBuffer(&response, &return_bytes);
        PutKeyValueMessageResponse_SerializeIntoBuffer(&response, &return_bytes);
        TcpFunction_SendBufferOverSocketWithRetry(socket, &return_bytes);
        PutKeyValueMessageResponse_DestroyBuffer(&return_bytes);
    }
    // TODO: Fill out failing case implementations later
    else
    {
        SizeAwareBuffer_PrintBuffer(Status_GetError(&status));
        Status_CleanUpResources(&status);
    }
}

