#include "common/StringBuffer.h"
#include <stdio.h>

void printString(StringBuffer* buffer)
{
    fwrite(buffer->string_pointer, sizeof(char), buffer->string_size, stdout);
}

void initializeStringBuffer(StringBuffer* buffer, char* string_pointer, uint32_t string_size)
{
    buffer->string_pointer = string_pointer;
    buffer->string_size = string_size;
}
