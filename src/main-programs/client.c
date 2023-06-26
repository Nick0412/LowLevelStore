#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include "network-utils/TcpFunctions.h"
#include "messages/PutKeyValueMessageRequest.h"

int main()
{
    int socket_pointer = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in socket_address;
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(15000);
    inet_aton("127.0.0.1", &socket_address.sin_addr);

    connect(socket_pointer, (const struct sockaddr*)&socket_address, sizeof(socket_address));

    PutKeyValueMessageRequest req = {
        .key = &(AugmentedBuffer) {
            .buffer_pointer = "keyA",
            .buffer_size = 4
        },
        .value = &(AugmentedBuffer) {
            .buffer_pointer = "valueB",
            .buffer_size = 6
        }
    };

    uint32_t message_size;
    calculatePutKeyValueMessageRequestSize(&req, &message_size);
    printf("Message size sending: %d\n", message_size);
    
    AugmentedBuffer buff = {
        .buffer_pointer = malloc(message_size),
        .buffer_size = message_size
    };

    serializePutKeyValueMessageRequest(&req, &buff);

    sendBufferOverSocketWithRetry(socket_pointer, &buff);

    free(buff.buffer_pointer);

    return 0;
}
