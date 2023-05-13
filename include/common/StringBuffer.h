#ifndef STRING_BUFFER_H
#define STRING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct StringBuffer
{
    char* string_pointer;
    uint32_t string_size;

} StringBuffer;

void printString(StringBuffer* buffer);

void initializeStringBuffer(StringBuffer* buffer, char* string_pointer, uint32_t string_size);

bool areStringBuffersEqual(StringBuffer* first, StringBuffer* second);

#endif
