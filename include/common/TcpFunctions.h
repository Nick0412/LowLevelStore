#ifndef TCP_FUNCTIONS_H
#define TCP_FUNCTIONS_H

#include "SizeAwareBuffer.h"

void TcpFunction_SendBufferOverSocketWithRetry(int socket_fd, SizeAwareBuffer* buffer);

/**
 * @brief Assumes the buffer size and pointer have been allocated beforehand. 
 * 
 */
void TcpFunction_ReceiveBufferOverSocketWithRetry(int socket_fd, 
    SizeAwareBuffer* buffer,
    uint32_t offset,
    uint32_t shift);

void TcpFunction_ReceiveBufferSizeOverSocket(int socket_fd, uint32_t* return_size);

#endif
