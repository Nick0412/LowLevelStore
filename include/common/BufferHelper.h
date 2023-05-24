#ifndef BUFFER_HELPER_H
#define BUFFER_HELPER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct AugmentedBuffer
{
    void* buffer_pointer;
    uint32_t buffer_size;
} AugmentedBuffer;

/**
 * @brief Copies a string into a buffer at a given offset. This assumes memory has been allocated 
 * for the buffer and for the string.
 * 
 * @param string The string to be copied from.
 * @param buffer The buffer to copy the string to.
 * @param offset The offset in the buffer to place the string.
 */
void placeStringInBuffer(AugmentedBuffer* string, AugmentedBuffer* buffer, uint32_t offset);

/**
 * @brief Places a 32 bit unsigned integer into buffer at a given offset in the buffer. This
 * function assumes that memory has been allocated for the buffer already. The value is placed
 * in the buffer with network byte order.
 * 
 * @param value The 32 bit uint to put in the buffer.
 * @param buffer Destination buffer.
 * @param offset Offset location in the buffer to place the uint.
 */
void place32BitUintInBuffer(uint32_t value, AugmentedBuffer* buffer, uint32_t offset);

/**
 * @brief Gets a 32 bit uint from a buffer. This assumes that the buffer has been allocated
 * and holds a 32 bit uint in network byte order.
 * 
 * @param buffer Buffer holding the designated integer.
 * @param offset The offset location of the integer in the buffer.
 * @param return_value The pointer holding the returned integer value.
 */
void get32BitUintFromBuffer(AugmentedBuffer* buffer, uint32_t offset, uint32_t* return_value);


/**
 * @brief Get a string at an offset from the buffer. This assumes that the string has had memory
 * allocated to hold the string and its size before the function call.
 * 
 * `return_string` must be properly allocated for.
 * 
 * - AugmentedBuffer (return_string)
 *   - void* (buffer_pointer) [must be allocated]
 *   - int (buffer_size) [must be allocated, implicitly allocated when AugmentedBuffer is made]
 * 
 * The `buffer_size` of `return_string` must be set to the correct size. The only way we know the
 * size of the string to copy over is due to `return_string->buffer_size`.
 * 
 * @param buffer The buffer to read from.
 * @param offset The offset location of where to read the string from.
 * @param return_string The returned string. This should have memory allocated to hold the
 * returning string and the size of the string.
 */
void getStringFromBuffer(AugmentedBuffer* buffer, uint32_t offset, AugmentedBuffer* return_string);

/**
 * @brief Compares 2 augmented buffers and checks if the sizes and memory are the same.
 * 
 */
bool areAugmentedBuffersSame(const AugmentedBuffer* first, const AugmentedBuffer* second);

#endif
