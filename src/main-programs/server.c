#include "datastore/InMemoryDataStore.h"
#include "network-utils/TcpFunctions.h"
#include "messages/Constants.h"
#include "business-logic/MessageHandler.h"

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdlib.h>

int main()
{
    int socket_pointer = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in socket_address;
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(20000);
    inet_aton("127.0.0.1", &socket_address.sin_addr);

    int reuse = 1;
    setsockopt(socket_pointer, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    bind(socket_pointer, (const struct sockaddr*)&socket_address, sizeof(socket_address));
    listen(socket_pointer, 1);

    struct sockaddr_in incoming_address;
    socklen_t address_length;
    int connected_socket = accept(socket_pointer, (struct sockaddr*)&incoming_address, &address_length);

    InMemoryDataStore store;
    initializeInMemoryDataStore(&store);

    while (true)
    {
        uint32_t message_size;
        receiveBufferSizeOverSocket(connected_socket, &message_size);


        SizeAwareBuffer incoming_message = {
            .raw_buffer = malloc(message_size),
            .buffer_size = message_size
        };
        SizeAwareBuffer_Place32BitValue(message_size, &incoming_message, MESSAGE_SIZE_OFFSET);

        receiveBufferOverSocketWithRetry(connected_socket, &incoming_message, MESSAGE_TYPE_OFFSET, 4);

        handleMessage(&incoming_message, &store, connected_socket);

        free(incoming_message.raw_buffer);
    }
}
