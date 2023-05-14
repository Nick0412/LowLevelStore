#include "common/Error.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

void testInitializeError()
{
    Error error;
    AugmentedBuffer buff = {
        .buffer_pointer = "test error",
        .buffer_size = 10
    };
    int32_t error_code = 34;

    initializeError(&error, error_code, &buff);

    // Compares if 2 strings are the same (expecting 0).
    int string_compare = strncmp(
        error.error_string->buffer_pointer,
        buff.buffer_pointer,
        buff.buffer_size);

    assert(error.error_code == 34);
    assert(string_compare == 0);
}

int main()
{
    printf("STARTING ERROR TEST\n");

    testInitializeError();
}
