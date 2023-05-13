#include "common/StringBuffer.h"
#include <stdio.h>
#include <string.h>

void printString(StringBuffer* buffer)
{
    printf("{\n\tString Pointer: ");
    fwrite(buffer->string_pointer, sizeof(char), buffer->string_size, stdout);
    printf("\n\tSize: %d\n}", buffer->string_size);
}

void initializeStringBuffer(StringBuffer* buffer, char* string_pointer, uint32_t string_size)
{
    buffer->string_pointer = string_pointer;
    buffer->string_size = string_size;
}

bool areStringBuffersEqual(StringBuffer* first, StringBuffer* second)
{
    return (first->string_size == second->string_size) &&
        (strncmp(first->string_pointer, second->string_pointer, first->string_size) == 0);
}
