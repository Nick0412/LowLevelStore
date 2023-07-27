#ifndef MESSAGES_COMMON_H
#define MESSAGES_COMMON_H

#define MESSAGE_SIZE_OFFSET 0
#define MESSAGE_TYPE_OFFSET 4
#define MESSAGE_DATA_OFFSET 8
// TODO: Need a better naming convention here. These constants are meant to represent the size
// of various fields in most messages. Like the type of a message is 4 bytes usually and so is the
// size field.
#define MESSAGE_SIZE_BYTE_SIZE 4
#define MESSAGE_TYPE_BYTE_SIZE 4
#define MESSAGE_SIZE_FIELD_BYTES 4

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
