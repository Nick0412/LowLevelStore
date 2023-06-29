#include "common/SizeAwareBuffer.h"

#include <stdio.h>
#include <stdlib.h>

void SizeAwareBuffer_AllocateBuffer(uint32_t number_of_bytes, SizeAwareBuffer* return_buffer)
{
    return_buffer->buffer_size = number_of_bytes;
    return_buffer->raw_buffer = malloc(number_of_bytes);
}

void SizeAwareBuffer_DestroyBuffer(SizeAwareBuffer* buffer)
{
    free(buffer->raw_buffer);
    buffer->raw_buffer = NULL;
    buffer->buffer_size = 0;
}

bool SizeAwareBuffer_AreContentsSame(const SizeAwareBuffer* buffer_1, const SizeAwareBuffer* buffer_2)
{
    return (buffer_1->buffer_size == buffer_2->buffer_size) &&
           (memcmp(buffer_1->raw_buffer, buffer_2->raw_buffer, buffer_1->buffer_size) == 0);
}

void SizeAwareBuffer_CopyBufferIntoOtherBuffer(const SizeAwareBuffer* source_buffer, uint32_t offset_into_source,
    uint32_t number_of_bytes, SizeAwareBuffer* destination_buffer, uint32_t offset_into_destination)
{
    if (offset_into_source >= source_buffer->buffer_size ||
        offset_into_source + number_of_bytes > source_buffer->buffer_size)
    {
        perror("The copy range is outside of the source_buffer's length.");
        return;
    }

    if (offset_into_destination >= destination_buffer->buffer_size ||
        offset_into_destination + number_of_bytes >= destination_buffer->buffer_size)
    {
        perror("The copy range is outside of the destination_buffer's length.");
        return;
    }

    memcpy(destination_buffer->raw_buffer + offset_into_destination,
        source_buffer->raw_buffer + offset_into_source, number_of_bytes);
}
