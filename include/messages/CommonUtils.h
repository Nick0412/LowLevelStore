#ifndef MESSAGES_COMMON_UTILS_H
#define MESSAGES_COMMON_UTILS_H

#include "common/BufferHelper.h"
#include "messages/MessageTypes.h"
#include <stdint.h>

/**
 * Precondition:
 *   - `message_bytes` must be at least 4 bytes long.
*/
void Messages_GetSizeFromBuffer(const AugmentedBuffer* message_bytes, uint32_t* return_message_bytes_size);

/**
 * Precondition:
 *   - `message_bytes` must be at least 8 bytes long.
*/
void Messages_GetTypeFromBuffer(const AugmentedBuffer* message_bytes, MessageType* return_message_type);

#endif
