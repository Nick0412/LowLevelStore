#ifndef MESSAGES_COMMON_H
#define MESSAGES_COMMON_H

#include "common/SizeAwareBuffer.h"
#include "messages/MessageTypes.h"
#include <stdint.h>

/**
 * Precondition:
 *   - `message_bytes` must be at least 4 bytes long.
*/
void Messages_GetSizeFromBuffer(const SizeAwareBuffer* message_bytes, uint32_t* return_message_bytes_size);

/**
 * Precondition:
 *   - `message_bytes` must be at least 8 bytes long.
*/
void Messages_GetTypeFromBuffer(const SizeAwareBuffer* message_bytes, MessageType* return_message_type);

#endif
