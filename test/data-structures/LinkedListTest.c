#include "data-structures/LinkedList.h"
#include <stdio.h>
#include <assert.h>

static void nullAssertion(const LinkedList* list)
{
    assert(list->head == NULL);
    assert(list->tail == NULL);
    assert(list->size == 0);
}

static void helperAssertion(const LinkedList* list,
    const LinkedListNode* head,
    const LinkedListNode* tail,
    uint32_t size)
{
    assert(list->head == head);
    assert(list->tail == tail);
    assert(list->size == size);
}

static void initTest()
{
    LinkedList list;
    LinkedList_Initialize(&list);

    nullAssertion(&list);
}

static void singleItemTest()
{
    LinkedList list;
    LinkedList_Initialize(&list);

    VoidBuffer buff = { .data = "12345", .number_of_bytes = 5 };
    LinkedList_InsertDataAtTail(&list, &buff);
    LinkedListNode* head = list.head;

    helperAssertion(&list, head, head, 1);
    VoidBuffer_AreEqual(&head->data, &buff);

    LinkedList_Destroy(&list);
    nullAssertion(&list);
}

static void multipleItemTest()
{
    LinkedList list;
    LinkedList_Initialize(&list);

    VoidBuffer buff1 = { .data = "123", .number_of_bytes = 3 };
    VoidBuffer buff2 = { .data = "test", .number_of_bytes = 4 };
    VoidBuffer buff3 = { .data = "a", .number_of_bytes = 1 };
    LinkedList_InsertDataAtTail(&list, &buff1);
    LinkedList_InsertDataAtTail(&list, &buff2);
    LinkedList_InsertDataAtTail(&list, &buff3);

    helperAssertion(&list, list.head, list.tail, 3);
    VoidBuffer_AreEqual(&list.head->data, &buff1);
    VoidBuffer_AreEqual(&list.head->next->data, &buff2);
    VoidBuffer_AreEqual(&list.tail->data, &buff3);

    LinkedList_Destroy(&list);
    nullAssertion(&list);
}

static void deleteItemSuccessTest()
{
    LinkedList list;
    LinkedList_Initialize(&list);

    VoidBuffer buff1 = { .data = "123", .number_of_bytes = 3 };
    VoidBuffer buff2 = { .data = "test", .number_of_bytes = 4 };
    VoidBuffer buff3 = { .data = "a", .number_of_bytes = 1 };
    LinkedList_InsertDataAtTail(&list, &buff1);
    LinkedList_InsertDataAtTail(&list, &buff2);
    LinkedList_InsertDataAtTail(&list, &buff3);
    Result res = LinkedList_DeleteData(&list, &buff3);

    assert(res.is_successful);
    helperAssertion(&list, list.head, list.tail, 2);
    VoidBuffer_AreEqual(&list.head->data, &buff1);
    VoidBuffer_AreEqual(&list.head->next->data, &buff2);
    VoidBuffer_AreEqual(&list.tail->data, &buff2);

    LinkedList_Destroy(&list);
    nullAssertion(&list);
}

static void deleteItemFailureTest()
{
    LinkedList list;
    LinkedList_Initialize(&list);

    VoidBuffer buff1 = { .data = "123", .number_of_bytes = 3 };
    VoidBuffer buff2 = { .data = "test", .number_of_bytes = 4 };
    VoidBuffer buff3 = { .data = "a", .number_of_bytes = 1 };
    LinkedList_InsertDataAtTail(&list, &buff1);
    LinkedList_InsertDataAtTail(&list, &buff2);
    
    // Try to delete buff3 when it is not in list
    Result res = LinkedList_DeleteData(&list, &buff3);
    SizeAwareBuffer expected_details = {
        .buffer_size = 43,
        .raw_buffer = (uint8_t*)"Failed to find data in linked list. Data: a"
    };

    assert(!res.is_successful);
    assert(res.error_type == ERROR_TYPE_LINKED_LIST);
    assert(res.error_code == LINKED_LIST_ERROR_DATA_NOT_FOUND);
    assert(SizeAwareBuffer_AreContentsSame(&res.error_details, &expected_details));

    helperAssertion(&list, list.head, list.tail, 2);
    VoidBuffer_AreEqual(&list.head->data, &buff1);
    VoidBuffer_AreEqual(&list.head->next->data, &buff2);
    VoidBuffer_AreEqual(&list.tail->data, &buff2);

    Result_Destroy(&res);
    LinkedList_Destroy(&list);
    nullAssertion(&list);
}

int main()
{
    printf("STARTING LINKED LIST TEST\n");

    initTest();

    singleItemTest();

    multipleItemTest();

    deleteItemSuccessTest();

    deleteItemFailureTest();
}

