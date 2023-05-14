#include "common/BufferHelper.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void testPlaceStringAndGetStringFromBuffer()
{
    AugmentedBuffer buffer = {
        .buffer_pointer = malloc(10),
        .buffer_size = 10
    };

    AugmentedBuffer input_string = {
        .buffer_pointer = "test1",
        .buffer_size = 5
    };

    uint32_t offset = 3;

    // We want to grab the string "test".
    AugmentedBuffer return_string = {
        .buffer_pointer = malloc(4),
        .buffer_size = 4
    };

    placeStringInBuffer(&input_string, &buffer, offset);
    getStringFromBuffer(&buffer, offset, &return_string);

    assert(strncmp(return_string.buffer_pointer, "test", return_string.buffer_size) == 0);

    free(buffer.buffer_pointer);
    free(return_string.buffer_pointer);
}

void testPlace32BitUintAndGetFromBuffer()
{
    AugmentedBuffer buffer = {
        .buffer_pointer = malloc(10),
        .buffer_size = 10
    };

    uint32_t value = 0xFECDBA98;
    uint32_t offset = 4;
    uint32_t return_value = 0;

    place32BitUintInBuffer(value, &buffer, offset);
    get32BitUintFromBuffer(&buffer, offset, &return_value);

    assert(return_value == value);

    free(buffer.buffer_pointer);
}

int main()
{
    printf("STARTING BUFFER HELPER TEST\n");

    testPlaceStringAndGetStringFromBuffer();

    testPlace32BitUintAndGetFromBuffer();
}
