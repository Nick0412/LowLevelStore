#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "messages/PutKeyValueMessageRequest.h"
#include "messages/Constants.h"
#include "messages/PutKeyValueMessageResponse.h"
#include "messages/GetValueMessageRequest.h"
#include "messages/GetValueMessageResponse.h"
#include "common/TcpFunctions.h"

int main(int argc, char** argv)
{
    (void)argc;

    int socket_pointer = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in socket_address = { .sin_family = AF_INET, .sin_port = htons(20000) };
    inet_aton("127.0.0.1", &socket_address.sin_addr);
    int reuse = 1;
    setsockopt(socket_pointer, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    connect(socket_pointer, (const struct sockaddr*)&socket_address, sizeof(socket_address));

    char* option = argv[1];
    if (strcmp(option, "insert") == 0)
    {
        char* key = argv[2];
        char* value = argv[3];

        PutKeyValueMessageRequest request = {
            .key = { .raw_buffer = (uint8_t*)key, .buffer_size = strlen(key) },
            .value = { .raw_buffer = (uint8_t*)value, .buffer_size = strlen(value) }
        };
        SizeAwareBuffer send_bytes;
        PutKeyValueMessageRequest_AllocateBuffer(&request, &send_bytes);
        PutKeyValueMessageRequest_SerializeIntoBuffer(&request, &send_bytes);
        TcpFunction_SendBufferOverSocketWithRetry(socket_pointer, &send_bytes);
        PutKeyValueMessageRequest_DestroyBuffer(&send_bytes);

        uint32_t return_size;
        SizeAwareBuffer return_message;
        PutKeyValueMessageResponse response;
        TcpFunction_ReceiveBufferSizeOverSocket(socket_pointer, &return_size);
        SizeAwareBuffer_AllocateBuffer(return_size, &return_message);
        SizeAwareBuffer_Place32BitValue(return_size, &return_message, MESSAGE_SIZE_OFFSET);
        TcpFunction_ReceiveBufferOverSocketWithRetry(socket_pointer,
            &return_message,
            MESSAGE_TYPE_OFFSET,
            MESSAGE_SIZE_BYTE_SIZE);
        PutKeyValueMessageResponse_AllocateMessage(&return_message, &response);
        PutKeyValueMessageResponse_Deserialize(&return_message, &response);
        printf("Insert response was %d\n", response.status);
        PutKeyValueMessageResponse_DestroyMessage(&response);
        SizeAwareBuffer_DestroyBuffer(&return_message);
    } 
    else if (strcmp(option, "get") == 0)
    {
        char* key = argv[2];

        GetValueMessageRequest request = {
            .key = { .raw_buffer = (uint8_t*)key, .buffer_size = strlen(key)}
        };
        SizeAwareBuffer send_bytes;
        GetValueMessageRequest_AllocateBuffer(&request, &send_bytes);
        GetValueMessageRequest_SerializeIntoBuffer(&request, &send_bytes);
        TcpFunction_SendBufferOverSocketWithRetry(socket_pointer, &send_bytes);
        GetValueMessageRequest_DestroyBuffer(&send_bytes);

        uint32_t return_size;
        SizeAwareBuffer return_message;
        GetValueMessageResponse response;
        TcpFunction_ReceiveBufferSizeOverSocket(socket_pointer, &return_size);
        SizeAwareBuffer_AllocateBuffer(return_size, &return_message);
        SizeAwareBuffer_Place32BitValue(return_size, &return_message, MESSAGE_SIZE_OFFSET);
        TcpFunction_ReceiveBufferOverSocketWithRetry(socket_pointer,
            &return_message,
            MESSAGE_TYPE_OFFSET,
            MESSAGE_SIZE_BYTE_SIZE);
        GetValueMessageResponse_AllocateMessage(&return_message, &response);
        GetValueMessageResponse_Deserialize(&return_message, &response);
        printf("Value was %.*s\n", response.value.buffer_size, response.value.raw_buffer);
        GetValueMessageResponse_DestroyMessage(&response);
        SizeAwareBuffer_DestroyBuffer(&return_message);
    }
    else
    {
        printf("WHAT ARE YOU DOING\n");
    }

    close(socket_pointer);
}
