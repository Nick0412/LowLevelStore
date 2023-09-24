#include <stdio.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <string.h>
#include <unistd.h>
#include "Header.h"

int main()
{
    Socket server_sock = socket(AF_INET, SOCK_STREAM, 0);
    SocketAddress server_addr = Socket_CreateIPv4InetAddress("127.0.0.1", 23000);
    Socket_BindAddressToServerSocket(server_sock, &server_addr);
    Socket_ListenOnServerSocket(server_sock, 2);

    PollFileDescriptor files_to_poll[3];
    files_to_poll[0].fd = server_sock;
    files_to_poll[0].events = POLLIN;
    nfds_t number_of_files = 1;

    int while_iter = 0;
    while (true)
    {
        printf("While iterarion: %d\n", while_iter);
        while_iter++;
        printf("Polling\n");
        int poll_status = poll(files_to_poll, number_of_files, -1);
        printf("Poll status: %d\n", poll_status);
        for (int i = 0; i < number_of_files; i++)
        {
            // printf("Iteration %d\n", i);
            if (files_to_poll[i].revents & POLLIN)
            {
                if (i == 0)
                {
                    printf("[0]: New Socket\n");
                    SocketAddress client_address;
                    socklen_t client_address_size;
                    int new_socket = accept(server_sock, (struct sockaddr*)&client_address, &client_address_size);

                    files_to_poll[number_of_files] = (PollFileDescriptor) {
                        .fd = new_socket,
                        .revents = 0x0001
                    };
                    number_of_files++;
                }
                else
                {
                    printf("[1]: Read Write\n");
                    char read_buffer[2];
                    char write_buffer[5];
                    int bytes_read = recv(files_to_poll[i].fd, read_buffer, 2, 0);
                    printf("[1]: Bytes Received %s\n", read_buffer);

                    if (bytes_read <= 0)
                    {
                        printf("[1]: Closing socket due to error\n");
                        close(files_to_poll[i].fd);
                        files_to_poll[i] = files_to_poll[number_of_files-1];
                        number_of_files--;
                        i--;
                    }
                    else
                    {   
                        write_buffer[0] = 'O';
                        write_buffer[1] = 'K';
                        write_buffer[2] = ' ';
                        write_buffer[3] = read_buffer[0];
                        write_buffer[4] = '\0';
                        printf("[1]: Bytes sent %s\n", write_buffer);
                        Socket_SendBytesWithRetries(files_to_poll[i].fd, (U8*)write_buffer, 5, 0);
                    }
                }
                files_to_poll[i].revents = 0;
            }
            printf("\n\n");
        }
    }

    return 0;
}
