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

void LinkedList_InsertShallowDataAtTail(LinkedList* list, VoidBuffer* data);

Result LinkedList_DeleteData(LinkedList* list, const VoidBuffer* data);

Result LinkedList_Internal_CreateFailedToFindDataResult(const VoidBuffer* buffer);

Result LinkedList_Internal_CreateSuccessResult();

void LinkedListNode_Internal_Print(const LinkedListNode* node);

void LinkedList_Internal_Print(const LinkedList* list);

// TODO: Return result indicating if linked list was deleted.
void LinkedList_DeleteNode(LinkedList* list, LinkedListNode* node);

bool LinkedList_Internal_IsNodeInList(const LinkedList* list, const LinkedListNode* node);

#endif
