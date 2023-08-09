#include "data-structures/VoidBuffer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void VoidBuffer_Allocate(uint32_t number_of_bytes, VoidBuffer* return_buffer)
{
    return_buffer->number_of_bytes = number_of_bytes;
    return_buffer->data = malloc(number_of_bytes);
}

void VoidBuffer_DeepCopy(const VoidBuffer* buffer, VoidBuffer* return_buffer)
{
    return_buffer->number_of_bytes = buffer->number_of_bytes;
    return_buffer->data = malloc(buffer->number_of_bytes);

    memcpy(return_buffer->data, buffer->data, buffer->number_of_bytes);
}

void VoidBuffer_ShallowCopy(VoidBuffer* buffer, VoidBuffer* return_buffer)
{
    return_buffer->number_of_bytes = buffer->number_of_bytes;
    return_buffer->data = buffer->data;
}

void VoidBuffer_Destroy(VoidBuffer* buffer)
{
    free(buffer->data);
    buffer->number_of_bytes = 0;
}

bool VoidBuffer_AreEqual(const VoidBuffer* buffer_1, const VoidBuffer* buffer_2)
{
    if (buffer_1->number_of_bytes != buffer_2->number_of_bytes)
    {
        return false;
    }

    if (memcmp(buffer_1->data, buffer_2->data, buffer_1->number_of_bytes) != 0)
    {
        return false;
    }

    return true;
}

void VoidBuffer_Print(const VoidBuffer* buffer)
{
    for (uint32_t i = 0; i < buffer->number_of_bytes; i++)
    {
        printf("%c", ((char*)buffer->data)[i]);
    }
    printf("\n");
}
