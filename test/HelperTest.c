#include "Helper.h"
#include <unistd.h>
#include <assert.h>

void print3Task(void* args)
{
    sleep(3);
    printf("Print 3 task\n");
}

void print5Task(void* args)
{
    sleep(5);
    printf("Print 5 task\n");
}

void testThreadPool()
{
    PRINT_TEST();

    printf("[Main Thread] Initializing Thread Pool\n");
    ThreadPool thread_pool;
    ThreadPool_Initialize(&thread_pool, 2);

    ThreadTask task1 = {
        .argument_size = 0,
        .argument = NULL,
        .function = print3Task
    };

    ThreadTask task2 = {
        .argument_size = 0,
        .argument = NULL,
        .function = print5Task
    };

    ThreadPool_AddWork(&thread_pool, &task1);
    ThreadPool_AddWork(&thread_pool, &task2);
    ThreadPool_AddWork(&thread_pool, &task1);
    ThreadPool_AddWork(&thread_pool, &task2);

    sleep(30);
}

void QueueTest_InitializeAndDestroy()
{
    PRINT_TEST();

    Queue queue;
    Queue_Initialize(&queue);
    assert(queue.head == NULL);
    assert(queue.tail == NULL);
    assert(queue.number_of_elements == 0);

    Queue_Destroy(&queue);
    assert(queue.head == NULL);
    assert(queue.tail == NULL);
    assert(queue.number_of_elements == 0);
}

void QueueTest_IsEmpty()
{
    PRINT_TEST();

    Queue queue;
    bool is_empty;
    Queue_Initialize(&queue);
    Queue_IsEmpty(&queue, &is_empty);
    assert(is_empty);

    Queue_PushBack(&queue, "data", 4);
    Queue_IsEmpty(&queue, &is_empty);
    assert(!is_empty);

    Queue_Destroy(&queue);
    assert(queue.head == NULL);
    assert(queue.tail == NULL);
    assert(queue.number_of_elements == 0);
}

void QueueTest_PushBackSingle()
{
    PRINT_TEST();

    Queue queue;
    U32 head_size;
    Queue_Initialize(&queue);
    Queue_PushBack(&queue, "test", 4);
    Queue_GetSizeOfHead(&queue, &head_size);
    assert(queue.number_of_elements == 1);
    assert(queue.head != NULL);
    assert(queue.tail != NULL);
    assert(head_size == 4);
    assert(Utility_AreBytesTheSame(queue.head->data_pointer, queue.tail->data_pointer, 4));
    assert(Utility_AreBytesTheSame(queue.head->data_pointer, "test", 4));

    Queue_Destroy(&queue);
    assert(queue.tail == NULL);
    assert(queue.head == NULL);
    assert(queue.number_of_elements == 0);
}

void QueueTest_PushBackTwo()
{
    PRINT_TEST();

    Queue queue;
    U32 head_size;
    Queue_Initialize(&queue);
    Queue_PushBack(&queue, "test", 4);
    Queue_PushBack(&queue, "bla", 3);
    Queue_GetSizeOfHead(&queue, &head_size);
    assert(queue.number_of_elements == 2);
    assert(queue.head != NULL);
    assert(queue.tail != NULL);
    assert(head_size == 4);
    assert(!Utility_AreBytesTheSame(queue.head->data_pointer, queue.tail->data_pointer, 4));
    assert(Utility_AreBytesTheSame(queue.head->data_pointer, "test", 4));
    assert(Utility_AreBytesTheSame(queue.tail->data_pointer, "bla", 3));

    Queue_Destroy(&queue);
    assert(queue.tail == NULL);
    assert(queue.head == NULL);
    assert(queue.number_of_elements == 0);
}

int main()
{
    printf("STARTING TESTS\n");
    
    // QueueTest_InitializeAndDestroy();
    // QueueTest_IsEmpty();
    // QueueTest_PushBackSingle();
    // QueueTest_PushBackTwo();

    testThreadPool();
}
