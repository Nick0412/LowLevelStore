#include "datastore/InMemoryDataStore.h"
#include "client-server/ServerHandler.h"

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int socket_pointer = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in socket_address = { .sin_family = AF_INET, .sin_port = htons(20000) };
    inet_aton("127.0.0.1", &socket_address.sin_addr);

    int reuse = 1;
    setsockopt(socket_pointer, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    bind(socket_pointer, (const struct sockaddr*)&socket_address, sizeof(socket_address));
    listen(socket_pointer, 1);

    InMemoryDataStore store;
    InMemoryDataStore_Initialize(&store);

    while (true)
    {
        struct sockaddr_in incoming_address;
        socklen_t address_length;
        int connected_socket = accept(socket_pointer,
            (struct sockaddr*)&incoming_address,
            &address_length);

        ServerHandler_HandleMessage(connected_socket, &store);

        close(connected_socket);
    }
}
