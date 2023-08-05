#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "VoidBuffer.h"
#include "common/Result.h"
#include <stdint.h>
#include <stdbool.h>

// Implementation for a doubly linked list.
typedef struct LinkedListNode
{
    struct LinkedListNode* prev;
    struct LinkedListNode* next;
    VoidBuffer data;
} LinkedListNode;

typedef struct LinkedList
{
    LinkedListNode* head;
    LinkedListNode* tail;
    uint32_t size;
} LinkedList;

void LinkedList_Initialize(LinkedList* list);

void LinkedList_Destroy(LinkedList* list);

void LinkedList_InsertDataAtTail(LinkedList* list, const VoidBuffer* data);

Result LinkedList_DeleteData(LinkedList* list, const VoidBuffer* data);

#endif
