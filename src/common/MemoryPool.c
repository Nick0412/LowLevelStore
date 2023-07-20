#include "common/MemoryPool.h"
#include <stdlib.h>
#include <stdio.h>

void MemoryPool_Initialize(MemoryPool* pool)
{
    pool->head = NULL;
    pool->number_of_nodes = 0;
}

void MemoryPool_AllocateMemorySize(MemoryPool* pool, uint32_t number_of_bytes_to_allocate, SizeAwareBuffer** return_buffer)
{
    MemoryNode* new_node = malloc(sizeof(MemoryNode));
    new_node->next = pool->head;
    SizeAwareBuffer_AllocateBuffer(number_of_bytes_to_allocate, &new_node->data);
    
    *return_buffer = &new_node->data;
    
    pool->head = new_node;
    pool->number_of_nodes++;
}

void MemoryPool_DestroyPool(MemoryPool* pool)
{
    MemoryNode* curr = pool->head;
    while (curr != NULL)
    {
        SizeAwareBuffer_DestroyBuffer(&curr->data);
        MemoryNode* temp = curr;
        curr = curr->next;

        free(temp);
        temp = NULL;
    }

    pool->number_of_nodes = 0;
    pool->head = NULL;
}

void MemoryPool_PrintNodeSizes(const MemoryPool* pool)
{
    MemoryNode* curr = pool->head;
    while (curr != NULL)
    {
        printf("%d -> ", curr->data.buffer_size);
        curr = curr->next;
    }
    printf("NULL\n");
}
