#include "common/SizeAwareBuffer.h"

#include <assert.h>
#include <stdio.h>

void verifyAllocateAndDestroyBuffer()
{
    SizeAwareBuffer buff;
    SizeAwareBuffer_AllocateBuffer(10, &buff);

    assert(buff.buffer_size == 10);
    assert(buff.raw_buffer != NULL);

    SizeAwareBuffer_DestroyBuffer(&buff);

    assert(buff.buffer_size == 0);
    assert(buff.raw_buffer == NULL);
}

void verifyContentsComparisonOnBuffersTrue()
{
    char data1[] = { 'a', 'b', 'c', 'd' };
    SizeAwareBuffer buff1 = {
        .buffer_size = 4,
        .raw_buffer = (uint8_t*)data1
    };

    char data2[] = { 'a', 'b', 'c', 'd' };
    SizeAwareBuffer buff2 = {
        .buffer_size = 4,
        .raw_buffer = (uint8_t*)data2
    };

    assert(SizeAwareBuffer_AreContentsSame(&buff1, &buff2));
}

void verifyContentsComparisonOnBuffersFalse()
{
    char data1[] = { 'a', 'b', 'c', 'd' };
    SizeAwareBuffer buff1 = {
        .buffer_size = 4,
        .raw_buffer = (uint8_t*)data1
    };

    char data2[] = { 'a', 'b', 'c', 'E' };
    SizeAwareBuffer buff2 = {
        .buffer_size = 4,
        .raw_buffer = (uint8_t*)data2
    };

    assert(!SizeAwareBuffer_AreContentsSame(&buff1, &buff2));
}

void verifyCopyIntoBuffer()
{
    char data1[] = { 'a', 'b', '*', '*', 'e' };
    SizeAwareBuffer buff1 = {
        .buffer_size = 5,
        .raw_buffer = (uint8_t*)data1
    };

    char data2[] = { 'c', 'd' };
    SizeAwareBuffer buff2 = {
        .buffer_size = 2,
        .raw_buffer = (uint8_t*)data2
    };

    char data3[] = { 'a', 'b', 'c', 'd', 'e' };
    SizeAwareBuffer buff3 = {
        .buffer_size = 5,
        .raw_buffer = (uint8_t*)data3
    };

    SizeAwareBuffer_CopyBufferIntoOtherBuffer(&buff2, 0, 2, &buff1, 2);

    assert(SizeAwareBuffer_AreContentsSame(&buff1, &buff3));
}

int main()
{
    printf("STARTING SIZE AWARE BUFFER TEST\n");

    verifyAllocateAndDestroyBuffer();

    verifyContentsComparisonOnBuffersTrue();

    verifyCopyIntoBuffer();
}
