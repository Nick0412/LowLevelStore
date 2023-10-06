#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "Helper.h"

/**
 * CLI (arguments)
 * 
 * InMemStoreClient client
 * Socket socket
 * SocketAddress
 * 
 * 
 * @return int 
 */

int main()
{
    /*
    Socket client_sock = socket(AF_INET, SOCK_STREAM, 0);
    SocketAddress address = Socket_CreateIPv4InetAddress("127.0.0.1", 23000);
    Socket_ConnectClientSocketToAddress(client_sock, address);

    for (int i = 0; i < 100; i++)
    {
        printf("Iteration %d\n", i);
        char read_buffer[5];
        char write_buffer[2] = {'8', '\0'};

        printf("Sending bytes: %s\n", write_buffer);
        Socket_SendBytesWithRetries(client_sock, (U8*)write_buffer, 2, 0);

        Socket_ReceiveBytesWithRetries(client_sock, (U8*)read_buffer, 5, 0);
        printf("Receiving bytes: %s\n", read_buffer);
        sleep(5);

        printf("\n\n");
    }
    */
}
