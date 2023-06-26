#include "network-utils/TcpFunctions.h"

#include <sys/socket.h>

void sendBufferOverSocketWithRetry(int socket_fd, AugmentedBuffer* buffer)
{
    uint32_t bytes_sent = 0;
    while (bytes_sent < buffer->buffer_size)
    {
        char* current_offset = (char*)buffer->buffer_pointer + bytes_sent;
        uint32_t bytes_remaining = buffer->buffer_size - bytes_sent;

        bytes_sent += send(socket_fd, current_offset, bytes_remaining, 0);
    }
}

void receiveBufferOverSocketWithRetry(int socket_fd, AugmentedBuffer* buffer, uint32_t offset)
{
    uint32_t bytes_received = 0;
    while (bytes_received < buffer->buffer_size)
    {
        char* current_offset = (char*)buffer->buffer_pointer + bytes_received + offset;
        uint32_t bytes_remaining = buffer->buffer_size - bytes_received;

        bytes_received += recv(socket_fd, current_offset, bytes_remaining, 0);
    }
}

void receiveBufferSizeOverSocket(int socket_fd, uint32_t* return_size)
{
    uint8_t size_bytes[4];
    AugmentedBuffer temp = {
        .buffer_pointer = size_bytes,
        .buffer_size = 4
    };

    receiveBufferOverSocketWithRetry(socket_fd, &temp, 0);
    get32BitUintFromBuffer(&temp, 0, return_size);
}
