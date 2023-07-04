#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#include "network-utils/TcpFunctions.h"
#include "messages/PutKeyValueMessageRequest.h"
#include "messages/Constants.h"
#include "messages/PutKeyValueMessageResponse.h"
#include "messages/GetValueMessageRequest.h"
#include "messages/GetValueMessageResponse.h"

int main()
{
    int socket_pointer = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in socket_address;
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(20000);
    inet_aton("127.0.0.1", &socket_address.sin_addr);
    int reuse = 1;
    setsockopt(socket_pointer, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    connect(socket_pointer, (const struct sockaddr*)&socket_address, sizeof(socket_address));

    while (1)
    {
        printf("Options: \n");
        printf("1. Put key value\n");
        printf("2. Get value \n");
        int number;
        scanf("%d", &number);
        fflush(stdin);

        switch (number)
        {
            case 1:
            {
                char key_buffer[50];
                char val_buffer[50];

                printf("Enter key: ");
                fgets(key_buffer, sizeof(key_buffer), stdin); 
                key_buffer[strcspn(key_buffer, "\n")] = 0;
                int key_len = strlen(key_buffer);

                printf("Enter value: ");
                fgets(val_buffer, 50, stdin);
                val_buffer[strcspn(val_buffer, "\n")] = 0;
                int val_len = strlen(val_buffer);

                PutKeyValueMessageRequest req = {
                    .key = &(SizeAwareBuffer) {
                        .raw_buffer = key_buffer,
                        .buffer_size = key_len
                    },
                    .value = &(SizeAwareBuffer) {
                        .raw_buffer = val_buffer,
                        .buffer_size = val_len
                    }
                };

                uint32_t req_size;
                calculatePutKeyValueMessageRequestSize(&req, &req_size);

                SizeAwareBuffer buff = {
                    .raw_buffer = malloc(req_size),
                    .buffer_size = req_size
                };
                serializePutKeyValueMessageRequest(&req, &buff);

                sendBufferOverSocketWithRetry(socket_pointer, &buff);
                free(buff.raw_buffer);

                uint32_t incoming_size;
                receiveBufferSizeOverSocket(socket_pointer, &incoming_size);
                SizeAwareBuffer incoming_buff = {
                    .raw_buffer = malloc(incoming_size),
                    .buffer_size = incoming_size
                };

                SizeAwareBuffer_Place32BitValue(incoming_size, &incoming_buff, MESSAGE_SIZE_OFFSET);
                receiveBufferOverSocketWithRetry(socket_pointer, &incoming_buff, MESSAGE_TYPE_OFFSET, 4);

                PutKeyValueMessageResponse res;
                deserializePutKeyValueMessageResponse(&incoming_buff, &res);
                free(incoming_buff.raw_buffer);

                break;
            }
            case 2:
            {
                char key_buffer[50];

                printf("Enter key to lookup: ");
                fgets(key_buffer, sizeof(key_buffer), stdin); 
                key_buffer[strcspn(key_buffer, "\n")] = 0;
                int key_len = strlen(key_buffer);

                GetValueMessageRequest req = {
                    .key = &(SizeAwareBuffer) {
                        .raw_buffer = key_buffer,
                        .buffer_size = key_len
                    }
                };

                uint32_t req_size;
                calculateGetValueMessageRequestSize(&req, &req_size);
                SizeAwareBuffer buff = {
                    .raw_buffer = malloc(req_size),
                    .buffer_size = req_size
                };

                serializeGetValueMessageRequest(&req, &buff);
                sendBufferOverSocketWithRetry(socket_pointer, &buff);
                free(buff.raw_buffer);

                uint32_t incoming_size;
                receiveBufferSizeOverSocket(socket_pointer, &incoming_size);
                SizeAwareBuffer incoming_buff = {
                    .raw_buffer = malloc(incoming_size),
                    .buffer_size = incoming_size
                };

                SizeAwareBuffer_Place32BitValue(incoming_size, &incoming_buff, MESSAGE_SIZE_OFFSET);
                receiveBufferOverSocketWithRetry(socket_pointer, &incoming_buff, MESSAGE_TYPE_OFFSET, 4);

                uint32_t value_size;
                SizeAwareBuffer_Get32BitValue(&incoming_buff, 12, &value_size);
                GetValueMessageResponse res = {
                    .value = &(SizeAwareBuffer) {
                        .raw_buffer = malloc(value_size),
                        .buffer_size = value_size
                    }
                };
                deserializeGetValueMessageResponse(&incoming_buff, &res);

                printf("Value is: ");
                fwrite(res.value->raw_buffer, sizeof(char), res.value->buffer_size, stdout);
                printf("\n");
                
                free(incoming_buff.raw_buffer);
                free(res.value->raw_buffer);

                break;
            }
            default:
            {
                break;
            }
        }
    }

    return 0;
}
