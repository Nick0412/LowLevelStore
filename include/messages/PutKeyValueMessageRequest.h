#ifndef PUT_KEY_VALUE_MESSAGE_REQUEST_H
#define PUT_KEY_VALUE_MESSAGE_REQUEST_H

#include <stdint.h>
#include "MessageTypes.h"
#include "common/BufferHelper.h"
#include "common/MemoryPool.h"

/**
 * Size     | Type      | Key Size  | Key String    | Value Size  | Value String
 * -----------------------------------------------------------------------------
 * 4 Bytes  | 4 Bytes   | 4 Bytes   | ?? Bytes      | 4 Bytes     | ?? Bytes
 */
typedef struct PutKeyValueMessageRequest
{
    AugmentedBuffer* key;
    AugmentedBuffer* value;

} PutKeyValueMessageRequest;

/**
 * @brief Calculates the size of a PutKeyValueMessageRequest message and stores the result
 * in return_size.
 * 
 * @param message The message to calculate the size of. Assumes this message is populated and
 * has been allocated.
 * @param return_size The resulting size of the message. 
 */
void calculatePutKeyValueMessageRequestSize(PutKeyValueMessageRequest* message, uint32_t* return_size);

/**
 * @brief Serialized the PutKeyValueMessageRequest and puts the result into the return_buffer. Assumes
 * the return_buffer has been properly allocated and the size is set.
 * 
 * @param message 
 * @param return_buffer 
 */
void serializePutKeyValueMessageRequest(PutKeyValueMessageRequest* message, AugmentedBuffer* return_buffer);

/**
 * @brief Deserializes the message and puts the result into return_message. 
 * This function allocates memory for the fields in return_message!! This is because we do not know the key
 * and value sizes until we read the buffer.
 * 
 * @param pool The memory pool which will manage memory allocations in this function.
 * @param buffer 
 * @param return_message 
 */
void deserializePutKeyValueMessageRequest(MemoryPoolList* pool, AugmentedBuffer* buffer, PutKeyValueMessageRequest* return_message);

#endif
