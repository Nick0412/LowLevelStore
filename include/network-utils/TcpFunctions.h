#ifndef TCP_FUNCTIONS_H
#define TCP_FUNCTIONS_H

#include "common/BufferHelper.h"

void sendBufferOverSocketWithRetry(int socket_fd, AugmentedBuffer* buffer);

/**
 * @brief Assumes the buffer size and pointer have been allocated beforehand. 
 * 
 */
void receiveBufferOverSocketWithRetry(int socket_fd, AugmentedBuffer* buffer, uint32_t offset);

void receiveBufferSizeOverSocket(int socket_fd, uint32_t* return_size);

#endif
