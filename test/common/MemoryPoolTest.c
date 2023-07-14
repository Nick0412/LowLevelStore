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
    printf("  - testZeroNodePool\n");
    
    MemoryPool pool;
    MemoryPool_Initialize(&pool);

    helperTestPoolIsCleanedUp(&pool);
}

void testMultiNodePool()
{
    printf("  - testZeroNodePool\n");

    MemoryPool pool;
    MemoryPool_Initialize(&pool);

    // MemoryPool_AllocateMemorySize(&pool, 10, )
}

int main()
{
    printf("STARTING MEMORY POOL TEST\n");

    testZeroNodePool();

    // testSingleNodePool();

    // testAllocateMemoryInPool();
}
