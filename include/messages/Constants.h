#ifndef MESSAGE_CONSTANTS_H
#define MESSAGE_CONSTANTS_H

#define MESSAGE_SIZE_OFFSET 0
#define MESSAGE_TYPE_OFFSET 4
#define MESSAGE_DATA_OFFSET 8

// TODO: Need a better naming convention here. These constants are meant to represent the size
// of various fields in most messages. Like the type of a message is 4 bytes usually and so is the
// size field.
#define MESSAGE_SIZE_BYTE_SIZE 4
#define MESSAGE_TYPE_BYTE_SIZE 4

#define MESSAGE_SIZE_FIELD_BYTES 4

#endif
