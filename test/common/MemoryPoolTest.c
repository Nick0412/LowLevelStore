#include "common/MemoryPool.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void helperTestPoolIsCleanedUp(MemoryPoolList* pool)
{
    assert(pool->number_of_nodes == 0);
    assert(pool->head == NULL);
    assert(pool->tail == NULL);
}

void testZeroNodePool()
{
    MemoryPoolList pool;
    initializeMemoryPool(&pool);

    assert(pool.number_of_nodes == 0);
    assert(pool.head == NULL);
    assert(pool.tail == NULL);

    destroyMemoryPool(&pool);
    helperTestPoolIsCleanedUp(&pool);
}

void testSingleNodePool()
{
    MemoryPoolList pool;
    initializeMemoryPool(&pool);

    void* mem_block = malloc(5);
    addMemoryPoolNode(&pool, 5, mem_block);

    assert(pool.number_of_nodes == 1);
    assert(pool.head != NULL);
    assert(pool.tail != NULL);

    destroyMemoryPool(&pool);
    helperTestPoolIsCleanedUp(&pool);
}

void testAllocateMemoryInPool()
{
    MemoryPoolList pool;
    SizeAwareBuffer buffer;
    char* test_string = "1234567890";

    initializeMemoryPool(&pool);
    allocateMemoryInPool(&pool, 10, &buffer);

    memcpy(buffer.raw_buffer, test_string, 10);
    
    assert(buffer.buffer_size == 10);
    assert(memcmp(buffer.raw_buffer, test_string, 10) == 0);

    destroyMemoryPool(&pool);
}

int main()
{
    printf("STARTING MEMORY POOL TEST\n");

    testZeroNodePool();

    testSingleNodePool();

    testAllocateMemoryInPool();
}
