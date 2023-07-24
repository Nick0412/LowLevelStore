#ifndef TCP_FUNCTIONS_H
#define TCP_FUNCTIONS_H

#include "SizeAwareBuffer.h"

void sendBufferOverSocketWithRetry(int socket_fd, SizeAwareBuffer* buffer);

/**
 * @brief Assumes the buffer size and pointer have been allocated beforehand. 
 * 
 */
void receiveBufferOverSocketWithRetry(int socket_fd, SizeAwareBuffer* buffer, uint32_t offset, uint32_t shift);

void receiveBufferSizeOverSocket(int socket_fd, uint32_t* return_size);

#endif
