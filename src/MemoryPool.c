#include "MemoryPool.h"
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
            free(curr);
            curr = next;
        }
        list->head = NULL;
        list->tail = NULL;
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
        MemoryPoolListNode* curr = list->head;
        while (curr != NULL)
        {
            printf("Pool node size: %d\n", curr->memory_block_size);
            curr = curr->next;
        }
    }
}