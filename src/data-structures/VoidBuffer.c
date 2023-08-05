#include "data-structures/VoidBuffer.h"
#include <string.h>
#include <stdlib.h>

void VoidBuffer_DeepCopy(const VoidBuffer* buffer, VoidBuffer* return_buffer)
{
    return_buffer->number_of_bytes = buffer->number_of_bytes;
    return_buffer->data = malloc(buffer->number_of_bytes);

    memcpy(return_buffer->data, buffer->data, buffer->number_of_bytes);
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
