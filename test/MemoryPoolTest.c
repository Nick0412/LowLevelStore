#include "MemoryPool.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
    printf("STARTING MEMORY POOL TEST\n");

    MemoryPoolList pool;
    initializeMemoryPool(&pool);

    assert(pool.number_of_nodes == 0);
    assert(pool.head == NULL);
    assert(pool.tail == NULL);

    void* mem_block = malloc(5);
    addMemoryPoolNode(&pool, 5, mem_block);

    assert(pool.number_of_nodes == 1);
    assert(pool.head != NULL);
    assert(pool.tail != NULL);

    void* mem_block_2 = malloc(10);
    addMemoryPoolNode(&pool, 10, mem_block_2);
    assert(pool.number_of_nodes == 2);

    destroyMemoryPool(&pool);
}