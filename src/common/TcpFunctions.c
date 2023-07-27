#include "common/TcpFunctions.h"

#include <sys/socket.h>
#include <stdio.h>

void TcpFunction_SendBufferOverSocketWithRetry(int socket_fd, SizeAwareBuffer* buffer)
{
    uint32_t bytes_sent = 0;
    while (bytes_sent < buffer->buffer_size)
    {
        char* current_offset = (char*)buffer->raw_buffer + bytes_sent;
        uint32_t bytes_remaining = buffer->buffer_size - bytes_sent;

        bytes_sent += send(socket_fd, current_offset, bytes_remaining, 0);
    }
}

void TcpFunction_ReceiveBufferOverSocketWithRetry(int socket_fd,
    SizeAwareBuffer* buffer,
    uint32_t offset,
    uint32_t shift)
{
    uint32_t bytes_received = 0;
    while (bytes_received < buffer->buffer_size - shift)
    {
        char* current_offset = (char*)buffer->raw_buffer + bytes_received + offset;
        uint32_t bytes_remaining = buffer->buffer_size - bytes_received;

        bytes_received += recv(socket_fd, current_offset, bytes_remaining, 0);
    }
}

void TcpFunction_ReceiveBufferSizeOverSocket(int socket_fd, uint32_t* return_size)
{
    uint8_t size_bytes[4];
    SizeAwareBuffer temp = {
        .raw_buffer = size_bytes,
        .buffer_size = 4
    };

    TcpFunction_ReceiveBufferOverSocketWithRetry(socket_fd, &temp, 0, 0);
    SizeAwareBuffer_Get32BitValue(&temp, 0, return_size);
}
