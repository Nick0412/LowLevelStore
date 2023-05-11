#include "common/Error.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

void testInitializeError()
{
    Error error;
    StringBuffer buff;
    int32_t error_code = 34;

    initializeStringBuffer(&buff, "test error", 10);
    initializeError(&error, error_code, &buff);

    // Compares if 2 strings are the same (expecting 0).
    int string_compare = strncmp(
        error.error_string->string_pointer,
        buff.string_pointer,
        buff.string_size);

    assert(error.error_code == 34);
    assert(string_compare == 0);
}

int main()
{
    printf("STARTING ERROR TEST\n");

    testInitializeError();
}
