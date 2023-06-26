#include "business-logic/MessageHandler.h"

#include "messages/MessageTypes.h"
#include "messages/PutKeyValueMessageRequest.h"
#include "messages/PutKeyValueMessageResponse.h"
#include "messages/GetValueMessageRequest.h"
#include "messages/GetValueMessageResponse.h"
#include <stdlib.h>
#include <stdio.h>

void handleMessage(AugmentedBuffer* incoming_message, InMemoryDataStore* store, int socket_pointer)
{
    // Get bytes 4 through 7 and cast to message type.
    uint32_t message_type_uint;
    get32BitUintFromBuffer(incoming_message, 4, &message_type_uint);
    MessageType type = (MessageType)message_type_uint;

    switch (type)
    {
        case PUT_KEY_VALUE_MESSAGE_REQUEST_TYPE:
        {
            handlePutKeyValueMessageRequest(incoming_message, store, socket_pointer);
            break;
        }
        case GET_VALUE_MESSAGE_REQUEST_TYPE:
        {
            handleGetValueMessageRequest(incoming_message, store, socket_pointer);
            break;
        }
        default:
        {
            break;
        }
    }
}

void handlePutKeyValueMessageRequest(AugmentedBuffer* incoming_message, InMemoryDataStore* store, int socket_pointer)
{
    PutKeyValueMessageRequest message;
    allocateMemoryForPutKeyValueMessageRequest(&message, incoming_message);
    deserializePutKeyValueMessageRequest(incoming_message, &message);

    KeyValueEntity entity = {
        .key = message.key,
        .value = message.value
    };
    insertKeyValuePair(store, &entity);
    
    PutKeyValueMessageResponse response = {
        .status = MESSAGE_SUCCESS
    };
    uint32_t size_of_response_buffer;
    calculatePutKeyValueMessageResponseSize(&response, &size_of_response_buffer);

    AugmentedBuffer response_buffer = {
        .buffer_pointer = malloc(size_of_response_buffer),
        .buffer_size = size_of_response_buffer
    };
    serializePutKeyValueMessageResponse(&response, &response_buffer);

    sendBufferOverSocketWithRetry(socket_pointer, &response_buffer);

    destroyMemoryForPutKeyValueMessageRequest(&message);
    free(response_buffer.buffer_pointer);
}

void handleGetValueMessageRequest(AugmentedBuffer* incoming_message, InMemoryDataStore* store, int socket_pointer)
{
    GetValueMessageRequest request;
    getValueMessageRequestAllocateMemory(incoming_message, &request);
    deserializeGetValueMessageRequest(incoming_message, &request);

    AugmentedBuffer* return_value;
    findValue(store, request.key, &return_value);

    GetValueMessageResponse response = {
        .value = return_value
    };
    uint32_t size_of_response_buffer;
    calculateGetValueMessageResponseSize(&response, &size_of_response_buffer);

    AugmentedBuffer response_buffer = {
        .buffer_pointer = malloc(size_of_response_buffer),
        .buffer_size = size_of_response_buffer
    };
    serializeGetValueMessageResponse(&response, &response_buffer);

    sendBufferOverSocketWithRetry(socket_pointer, &response_buffer);
    
    getValueMessageRequestDestroyMemory(&request);
    free(response_buffer.buffer_pointer);
}
