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

void verifyPlaceStringInBufferAndGet()
{
    char string_data[] = { 'a', 'b', 'c' };
    SizeAwareBuffer string = {
        .raw_buffer = (uint8_t*)string_data,
        .buffer_size = 3
    };

    char other_data[] = { '1', '2', '3', '4', '5', '6' };
    SizeAwareBuffer destination = {
        .raw_buffer = (uint8_t*)other_data,
        .buffer_size = 6
    };

    SizeAwareBuffer_PlaceStringInBuffer(&string, &destination, 3);

    uint8_t expected[] = { '1', '2', '3', 'a', 'b', 'c' };
    assert(memcmp(destination.raw_buffer, expected, 6) == 0);
}

void verifyPlace32BitValueAndGet()
{
    uint32_t value = 0xFEDCBA98;
    uint32_t offset = 2;
    uint8_t data[] = { 'a', 'b', 'c', 'w', 'x', 'y', 'z', 'e' };
    SizeAwareBuffer buff = {
        .raw_buffer = data,
        .buffer_size = 8
    };

    SizeAwareBuffer_Place32BitValue(value, &buff, offset);

    uint32_t return_value;
    SizeAwareBuffer_Get32BitValue(&buff, offset, &return_value);

    assert(buff.raw_buffer[2] == 0xFE);
    assert(buff.raw_buffer[3] == 0xDC);
    assert(buff.raw_buffer[4] == 0xBA);
    assert(buff.raw_buffer[5] == 0x98);
    assert(return_value == value);
}

int main()
{
    printf("STARTING SIZE AWARE BUFFER TEST\n");

    verifyAllocateAndDestroyBuffer();

    verifyContentsComparisonOnBuffersTrue();

    verifyCopyIntoBuffer();

    verifyPlaceStringInBufferAndGet();

    verifyPlace32BitValueAndGet();
}
