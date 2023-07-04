#include "common/MemoryPool.h"
#include <stdlib.h>
#include <stdio.h>

// TODO: Check for pointers being null;
void initializeMemoryPool(MemoryPoolList* list)
{
    list->head = NULL;
    list->tail = NULL;
    list->number_of_nodes = 0;
}

void addMemoryPoolNode(MemoryPoolList* list, uint32_t memory_block_size, void* memory_block_pointer)
{
    MemoryPoolListNode* new_node = malloc(sizeof(MemoryPoolListNode));
    new_node->memory_block_pointer = memory_block_pointer;
    new_node->memory_block_size = memory_block_size;
    new_node->next = NULL;

    if (list->number_of_nodes == 0)
    {
        list->head = new_node;
        list->tail = new_node;
        list->number_of_nodes = 1;
    }
    else
    {
        list->tail->next = new_node;
        list->tail = new_node;
        list->number_of_nodes++;
    }
}

void allocateMemoryInPool(MemoryPoolList* list, uint32_t memory_block_size, SizeAwareBuffer* output_augmented_buffer)
{
    // Build linked list node with contents.
    MemoryPoolListNode* new_node = malloc(sizeof(MemoryPoolListNode));
    void* memory_block = malloc(memory_block_size);
    new_node->memory_block_pointer = memory_block;
    new_node->memory_block_size = memory_block_size;
    new_node->next = NULL;

    // Attach linked list node correctly to tail and whether the list is empty or not.
    if (list->number_of_nodes == 0)
    {
        list->head = new_node;
        list->tail = new_node;
        list->number_of_nodes = 1;
    }
    else
    {
        list->tail->next = new_node;
        list->tail = new_node;
        list->number_of_nodes++;
    }

    // Build SizeAwareBuffer return value.
    output_augmented_buffer->raw_buffer = memory_block;
    output_augmented_buffer->buffer_size = memory_block_size;
}

void destroyMemoryPool(MemoryPoolList* list)
{
    if (list->number_of_nodes == 0)
    {
        return;
    }
    else
    {
        MemoryPoolListNode* curr = list->head;
        while (curr != NULL)
        {
            MemoryPoolListNode* next = curr->next;
            free(curr->memory_block_pointer);
            curr->memory_block_size = 0;
            free(curr);
            curr = next;
        }
        list->head = NULL;
        list->tail = NULL;
        list->number_of_nodes = 0;
    }
}

void printMemoryPoolSizes(MemoryPoolList* list)
{
    if (list->number_of_nodes == 0)
    {
        printf("List is empty.\n");
    }
    else
    {
        printf("Number of memory pool nodes: %d\n", list->number_of_nodes);
        MemoryPoolListNode* curr = list->head;
        while (curr != NULL)
        {
            printf("Pool node size: %d\n", curr->memory_block_size);
            curr = curr->next;
        }
    }
}
