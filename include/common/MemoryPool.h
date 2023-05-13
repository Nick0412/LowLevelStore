#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <stdint.h>

/**
 * A memory pool linked list used to hold and store all the memory that is dynamically allocated
 * during the life of the program. At the end of the program the user will destroy the memory pool.
 * 
 * This is a naive first attempt to have some sort of central memory management.
 */
typedef struct MemoryPoolListNode
{
    struct MemoryPoolListNode* next;
    void* memory_block_pointer;
    uint32_t memory_block_size;

} MemoryPoolListNode;

typedef struct MemoryPoolList
{
    MemoryPoolListNode* head;
    MemoryPoolListNode* tail;
    uint32_t number_of_nodes;
} MemoryPoolList;

void initializeMemoryPool(MemoryPoolList* list);

void addMemoryPoolNode(MemoryPoolList* list, uint32_t memory_block_size, void* memory_block_pointer);

void destroyMemoryPool(MemoryPoolList* list);

void printMemoryPoolSizes(MemoryPoolList* list);

#endif
