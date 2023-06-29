#ifndef SIZE_AWARE_BUFFER_H
#define SIZE_AWARE_BUFFER_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct SizeAwareBuffer
{
    uint8_t* raw_buffer;
    uint32_t buffer_size;

} SizeAwareBuffer;

/**
 * Precondition:
 *   - the struct `return_buffer` (not its members) must be allocated (i.e. dereferencing the struct pointer 
 *     must be valid)
*/
void SizeAwareBuffer_AllocateBuffer(uint32_t number_of_bytes, SizeAwareBuffer* return_buffer);

void SizeAwareBuffer_DestroyBuffer(SizeAwareBuffer* buffer);

bool SizeAwareBuffer_AreContentsSame(const SizeAwareBuffer* buffer_1, const SizeAwareBuffer* buffer_2);

/**
 * Copies the data between
 * source_buffer->raw_buffer[offset_into_source] source_buffer->raw_buffer[offset_into_source + number_of_bytes]
 * into starting position destination_buffer->raw_buffer[offset_into_destination].
 * 
 * Precondition:
 *   - `offset_into_source` must be in the range [0, source_buffer->buffer_size]
 *   - `offset_into_source` + number_of_bytes must be less than source_buffer->buffer_size
 *   - `offset_into_destination` must be in the range [0, destination_buffer->buffer_size]
 *   - `offset_into_destination` + number_of_bytes must be less than destination_buffer->buffer_size
 * 
*/
void SizeAwareBuffer_CopyBufferIntoOtherBuffer(const SizeAwareBuffer* source_buffer, uint32_t offset_into_source,
    uint32_t number_of_bytes, SizeAwareBuffer* destination_buffer, uint32_t offset_into_destination);

#endif
