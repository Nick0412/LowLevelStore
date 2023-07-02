#include "common/SizeAwareBuffer.h"
#include "common/Constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

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
    if (offset_into_source > source_buffer->buffer_size ||
        offset_into_source + number_of_bytes > source_buffer->buffer_size)
    {
        perror("The copy range is outside of the source_buffer's length.");
        return;
    }

    if (offset_into_destination > destination_buffer->buffer_size ||
        offset_into_destination + number_of_bytes > destination_buffer->buffer_size)
    {
        perror("The copy range is outside of the destination_buffer's length.");
        return;
    }

    memcpy(destination_buffer->raw_buffer + offset_into_destination,
        source_buffer->raw_buffer + offset_into_source, number_of_bytes);
}

void SizeAwareBuffer_PlaceStringInBuffer(const SizeAwareBuffer* string, SizeAwareBuffer* destination_buffer,
    uint32_t offset_into_destination)
{
    SizeAwareBuffer_CopyBufferIntoOtherBuffer(string, 0, string->buffer_size,
        destination_buffer, offset_into_destination);
}

void SizeAwareBuffer_Place32BitValue(uint32_t value, SizeAwareBuffer* destination_buffer,
    uint32_t offset_into_destination)
{
    uint32_t network_value = htonl(value);

    destination_buffer->raw_buffer[offset_into_destination] = network_value & 0xFF;
    destination_buffer->raw_buffer[offset_into_destination + 1] = (network_value >> 8) & 0xFF;
    destination_buffer->raw_buffer[offset_into_destination + 2] = (network_value >> 16) & 0xFF;
    destination_buffer->raw_buffer[offset_into_destination + 3] = (network_value >> 24) & 0xFF;
}

void SizeAwareBuffer_GetStringFromBuffer(const SizeAwareBuffer* source_buffer, uint32_t offset_into_source,
    SizeAwareBuffer* return_string)
{
    if (offset_into_source + return_string->buffer_size > source_buffer->buffer_size)
    {
        perror("String is out of bounds when trying to fetch from buffer.");
        return;
    }

    memcpy(return_string->raw_buffer, source_buffer->raw_buffer + offset_into_source, return_string->buffer_size);
}

void SizeAwareBuffer_Get32BitValue(const SizeAwareBuffer* source_buffer, uint32_t offset_into_source,
    uint32_t* return_value)
{
    if (offset_into_source + CONSTANT_UINT_32_SIZE > source_buffer->buffer_size)
    {
        perror("32 bit value is out of bounds when trying to fetch from buffer.");
        return;
    }

    *return_value = (source_buffer->raw_buffer[offset_into_source + 3] & 0xFF) << 24 | 
                    (source_buffer->raw_buffer[offset_into_source + 2] & 0xFF) << 16 | 
                    (source_buffer->raw_buffer[offset_into_source + 1] & 0xFF) << 8 |
                    (source_buffer->raw_buffer[offset_into_source] & 0xFF);

    *return_value = ntohl(*return_value);
}

void SizeAwareBuffer_PrintBuffer(const SizeAwareBuffer* buffer)
{
    printf("Buffer Size: %d\n", buffer->buffer_size);
    printf("Buffer Content: ");
    fwrite(buffer->raw_buffer, sizeof(uint8_t), buffer->buffer_size, stdout);
    printf("\n");
}
