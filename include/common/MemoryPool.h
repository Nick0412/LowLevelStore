#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <stdint.h>
#include "BufferHelper.h"

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

/**
 * [Deprecated]
 * 
 * Adds a new node to the linked list and attaches the size and memory pointer to that node.
 * This function is outdated as it expects the user to allocate memory before calling this function.
*/
void addMemoryPoolNode(MemoryPoolList* list, uint32_t memory_block_size, void* memory_block_pointer);

/**
 * @brief This function allocates memory and attaches it to the memory pool list. It will then return an 
 * AugmentedBuffer containing the size of the memory allocated and it's location.
 * 
 * @param list The memory pool list to attach our new memory to.
 * @param memory_block_size How large the memory allocated should be.
 * @param output_augmented_buffer A returned pointer containing the size of the memory block and it's location.
 * 
 * Note: the memory pool node and augmented buffer structs look similar.
 */
void allocateMemoryInPool(MemoryPoolList* list, uint32_t memory_block_size, AugmentedBuffer* output_augmented_buffer);

void destroyMemoryPool(MemoryPoolList* list);

void printMemoryPoolSizes(MemoryPoolList* list);

#endif
