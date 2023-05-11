#include "common/StringBuffer.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void testStringBufferInitialization()
{
    StringBuffer buffer;
    char* test_string = "this is a test string.";
    uint32_t string_size = strlen(test_string);

    initializeStringBuffer(&buffer, test_string, string_size);

    assert(buffer.string_size == 22);
    assert(buffer.string_pointer == test_string);
}

int main()
{
    printf("STARTING STRING BUFFER TEST\n");

    testStringBufferInitialization();
}
