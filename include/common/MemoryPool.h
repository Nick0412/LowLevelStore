#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <stdint.h>
#include "SizeAwareBuffer.h"

/**
 * A memory pool linked list used to hold and store all the memory that is dynamically allocated
 * during the life of the program. At the end of the program the user will destroy the memory pool.
 * 
 * This is a naive first attempt to have some sort of central memory management.
 */
typedef struct MemoryNode
{
    struct MemoryNode* next;
    SizeAwareBuffer data;
} MemoryNode;

typedef struct MemoryPool
{
    MemoryNode* head;
    uint32_t number_of_nodes;
} MemoryPool;

void MemoryPool_Initialize(MemoryPool* pool);

/**
 * Allocate memory in the pool of size `number_of_bytes_to_allocate` and return a buffer to that location.
 * The allocated memory is in a node that gets associated with the pool. Thee returned buffer's raw_buffer points
 * directly to the memory pool.
*/
void MemoryPool_AllocateMemorySize(MemoryPool* pool, uint32_t number_of_bytes_to_allocate, SizeAwareBuffer* return_buffer);

void MemoryPool_DestroyPool(MemoryPool* pool);

void MemoryPool_PrintNodeSizes(const MemoryPool* pool);

#endif
