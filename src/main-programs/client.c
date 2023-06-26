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

    connect(socket_pointer, (const struct sockaddr*)&socket_address, sizeof(socket_address));

    while (1)
    {



        int number;
        scanf("%d", &number);
        fflush(stdin);

        switch (number)
        {
            case 1:
            {
                char key_buffer[50];
                char val_buffer[50];

                fgets(key_buffer, sizeof(key_buffer), stdin); 
                key_buffer[strcspn(key_buffer, "\n")] = 0;
                int key_len = strlen(key_buffer);

                fgets(val_buffer, 50, stdin);
                val_buffer[strcspn(val_buffer, "\n")] = 0;
                int val_len = strlen(val_buffer);

                PutKeyValueMessageRequest req = {
                    .key = &(AugmentedBuffer) {
                        .buffer_pointer = key_buffer,
                        .buffer_size = key_len
                    },
                    .value = &(AugmentedBuffer) {
                        .buffer_pointer = val_buffer,
                        .buffer_size = val_len
                    }
                };

                uint32_t req_size;
                calculatePutKeyValueMessageRequestSize(&req, &req_size);

                AugmentedBuffer buff = {
                    .buffer_pointer = malloc(req_size),
                    .buffer_size = req_size
                };
                serializePutKeyValueMessageRequest(&req, &buff);

                sendBufferOverSocketWithRetry(socket_pointer, &buff);
                free(buff.buffer_pointer);

                uint32_t incoming_size;
                receiveBufferSizeOverSocket(socket_pointer, &incoming_size);
                AugmentedBuffer incoming_buff = {
                    .buffer_pointer = malloc(incoming_size),
                    .buffer_size = incoming_size
                };

                place32BitUintInBuffer(incoming_size, &incoming_buff, MESSAGE_SIZE_OFFSET);
                receiveBufferOverSocketWithRetry(socket_pointer, &incoming_buff, MESSAGE_TYPE_OFFSET, 4);

                PutKeyValueMessageResponse res;
                deserializePutKeyValueMessageResponse(&incoming_buff, &res);
                free(incoming_buff.buffer_pointer);

                break;
            }
            case 2:
            {
                char key_buffer[50];

                fgets(key_buffer, sizeof(key_buffer), stdin); 
                key_buffer[strcspn(key_buffer, "\n")] = 0;
                int key_len = strlen(key_buffer);

                GetValueMessageRequest req = {
                    .key = &(AugmentedBuffer) {
                        .buffer_pointer = key_buffer,
                        .buffer_size = key_len
                    }
                };

                uint32_t req_size;
                calculateGetValueMessageRequestSize(&req, &req_size);
                AugmentedBuffer buff = {
                    .buffer_pointer = malloc(req_size),
                    .buffer_size = req_size
                };

                serializeGetValueMessageRequest(&req, &buff);
                sendBufferOverSocketWithRetry(socket_pointer, &buff);
                free(buff.buffer_pointer);

                uint32_t incoming_size;
                receiveBufferSizeOverSocket(socket_pointer, &incoming_size);
                AugmentedBuffer incoming_buff = {
                    .buffer_pointer = malloc(incoming_size),
                    .buffer_size = incoming_size
                };

                place32BitUintInBuffer(incoming_size, &incoming_buff, MESSAGE_SIZE_OFFSET);
                receiveBufferOverSocketWithRetry(socket_pointer, &incoming_buff, MESSAGE_TYPE_OFFSET, 4);

                uint32_t value_size;
                get32BitUintFromBuffer(&incoming_buff, 12, &value_size);
                GetValueMessageResponse res = {
                    .value = &(AugmentedBuffer) {
                        .buffer_pointer = malloc(value_size),
                        .buffer_size = value_size
                    }
                };
                deserializeGetValueMessageResponse(&incoming_buff, &res);


                fwrite(res.value->buffer_pointer, sizeof(char), res.value->buffer_size, stdout);

                
                free(incoming_buff.buffer_pointer);
                free(res.value->buffer_pointer);

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
