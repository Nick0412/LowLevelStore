#include "common/MemoryPool.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// TODO: Finish tests for this file

void helperTestPoolIsCleanedUp(MemoryPool* pool)
{
    assert(pool->number_of_nodes == 0);
    assert(pool->head == NULL);
}

void testZeroNodePool()
{    
    MemoryPool pool;
    MemoryPool_Initialize(&pool);

    helperTestPoolIsCleanedUp(&pool);
}

void testMultiNodePool()
{
    MemoryPool pool;
    SizeAwareBuffer* buffer_pointer;
    SizeAwareBuffer* buffer_pointer_2;
    MemoryPool_Initialize(&pool);
    MemoryPool_AllocateMemorySize(&pool, 10, &buffer_pointer);
    MemoryPool_AllocateMemorySize(&pool, 20, &buffer_pointer_2);
    buffer_pointer_2->raw_buffer[2] = 0xAB;

    assert(pool.number_of_nodes == 2);
    assert(SizeAwareBuffer_AreContentsSame(&pool.head->next->data, buffer_pointer));
    assert(SizeAwareBuffer_AreContentsSame(&pool.head->data, buffer_pointer_2));
    assert(pool.head->data.raw_buffer[2] == 0xAB);

    MemoryPool_DestroyPool(&pool);
    helperTestPoolIsCleanedUp(&pool);
}

int main()
{
    printf("STARTING MEMORY POOL TEST\n");

    testZeroNodePool();

    testMultiNodePool();
}
