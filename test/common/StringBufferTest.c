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

void testStringBuffersAreEqual()
{
    StringBuffer buffer;
    char* test_string = "this is a test string.";
    uint32_t string_size = strlen(test_string);

    StringBuffer buffer_2;
    char* test_string_2 = "this is a test string.";
    uint32_t string_size_2 = strlen(test_string_2);

    initializeStringBuffer(&buffer, test_string, string_size);
    initializeStringBuffer(&buffer_2, test_string_2, string_size_2);

    assert(areStringBuffersEqual(&buffer, &buffer_2));
}

int main()
{
    printf("STARTING STRING BUFFER TEST\n");

    testStringBufferInitialization();

    testStringBuffersAreEqual();
}
