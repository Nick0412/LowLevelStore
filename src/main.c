#include <stdio.h>
#include "MemoryPool.h"
#include "stdlib.h"

int main()
{
    MemoryPoolList list;
    initializeMemoryPool(&list);

    void* test_mem_1 = malloc(5);
    void* test_mem_2 = malloc(10);
    void* test_mem_3 = malloc(20);

    printMemoryPoolSizes(&list);
    addMemoryPoolNode(&list, 5, test_mem_1);
    addMemoryPoolNode(&list, 10, test_mem_2);
    addMemoryPoolNode(&list, 20, test_mem_3);
    printMemoryPoolSizes(&list);

    destroyMemoryPool(&list);
}