#include "data-structures/LinkedList.h"
#include <stdlib.h>
#include <stdio.h>

Result LinkedList_Internal_CreateSuccessResult()
{
    Result res = {
        .is_successful = true,
        .error_code = 0,
        .error_type = ERROR_TYPE_EMPTY,
        .error_details = { .buffer_size = 0, .raw_buffer = NULL }
    };

    return res;
}

Result LinkedList_Internal_CreateFailedToFindDataResult(const VoidBuffer* buffer)
{
    // TODO: make this error message format into a function
    char* message_format = "Failed to find data in linked list. Data: %.*s";
    uint32_t num_message_bytes = snprintf(NULL,
        0,
        message_format,
        buffer->number_of_bytes,
        (char*)buffer->data);

    SizeAwareBuffer error_details;
    SizeAwareBuffer_AllocateBuffer(num_message_bytes, &error_details);
    snprintf((char*)error_details.raw_buffer,
        num_message_bytes+1,
        message_format,
        buffer->number_of_bytes,
        (char*)buffer->data);

    Result res = {
        .error_type = ERROR_TYPE_LINKED_LIST,
        .error_code = LINKED_LIST_ERROR_DATA_NOT_FOUND,
        .is_successful = false,
        .error_details = error_details
    };

    return res;
}

void LinkedList_Initialize(LinkedList* list)
{
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
}

void LinkedList_Destroy(LinkedList* list)
{
    LinkedListNode* current = list->head;
    LinkedListNode* next = (current == NULL ? NULL : current->next);

    while (current != NULL)
    {
        VoidBuffer_Destroy(&current->data);
        free(current);

        current = next;
        next = (next == NULL ? NULL : next->next);
    }

    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
}

void LinkedList_InsertDataAtTail(LinkedList* list, const VoidBuffer* data)
{
    // Create new node
    LinkedListNode* temp = malloc(sizeof(LinkedListNode));
    VoidBuffer_DeepCopy(data, &temp->data);

    if (list->size == 0)
    {
        list->head = temp;
        list->tail = temp;

        temp->next = NULL;
        temp->prev = NULL;
    }
    else
    {
        list->tail->next = temp;
        temp->prev = list->tail;
        temp->next = NULL;
        list->tail = temp;
    }

    list->size++;
}

void LinkedList_InsertShallowDataAtTail(LinkedList* list, VoidBuffer* data)
{
    LinkedListNode* temp = malloc(sizeof(LinkedListNode));
    VoidBuffer_ShallowCopy(data, &temp->data);

    if (list->size == 0)
    {
        list->head = temp;
        list->tail = temp;

        temp->next = NULL;
        temp->prev = NULL;
    }
    else
    {
        list->tail->next = temp;
        temp->prev = list->tail;
        temp->next = NULL;
        list->tail = temp;
    }

    list->size++;
}

Result LinkedList_DeleteData(LinkedList* list, const VoidBuffer* buffer)
{
    LinkedListNode* current = list->head;
    while (current != NULL)
    {
        if (VoidBuffer_AreEqual(buffer, &current->data))
        {
            if (current == list->head) 
                list->head = current->next;
            if (current == list->tail) 
                list->tail = current->prev;
            if (current->prev != NULL) 
                current->prev->next = current->next;
            if (current->next != NULL)
                current->next->prev = current->prev;

            VoidBuffer_Destroy(&current->data);
            free(current);
            list->size--;
            return LinkedList_Internal_CreateSuccessResult();
        }

        current = current->next;
    }

    return LinkedList_Internal_CreateFailedToFindDataResult(buffer);
}

void LinkedListNode_Internal_Print(const LinkedListNode* node)
{
    VoidBuffer_Print(&node->data);
}

void LinkedList_Internal_Print(const LinkedList* list)
{
    LinkedListNode* current = list->head;
    while (current != NULL)
    {
        LinkedListNode_Internal_Print(current);
        printf("|\nV\n");
    }
    printf("NULL\n");
}

void LinkedList_DeleteNode(LinkedList* list, LinkedListNode* node)
{
    LinkedListNode* current = list->head;
    while (current != NULL)
    {
        if (node == current)
        {
            if (current == list->head) 
                list->head = current->next;
            if (current == list->tail) 
                list->tail = current->prev;
            if (current->prev != NULL) 
                current->prev->next = current->next;
            if (current->next != NULL)
                current->next->prev = current->prev;
            
            VoidBuffer_Destroy(&node->data);
            free(node);
            list->size--;
        }
    }
}

bool LinkedList_Internal_IsNodeInList(const LinkedList* list, const LinkedListNode* node)
{
    LinkedListNode* curr = list->head;
    while (curr != NULL)
    {
        if (node == curr)
        {
            return true;
        }
        curr = curr->next;
    }
    return false;
}
