#include "Helper.h"
#include <unistd.h>
#include <assert.h>

void print3Task(void* args)
{
    (void)args;
    sleep(3);
    printf("Print 3 task\n");
}

void print5Task(void* args)
{
    (void)args;
    sleep(5);
    printf("Print 5 task\n");
}

void testThreadPool()
{
    PP_PRINT_TEST();

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
    PP_PRINT_TEST();

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
    PP_PRINT_TEST();

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
    PP_PRINT_TEST();

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
    PP_PRINT_TEST();

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

void get_value_request_verify_serialized_data_is_correct()
{
    PP_PRINT_TEST();

    GetValueRequest request = {
        .key = { .message = (U8*)"hello", .size = 5 }
    };
    SizedMessage request_bytes;
    GetValueRequest_Serialize(&request, &request_bytes);

    U32 message_size = sizeof(U32) * 2 + 5;
    U32 message_type;
    U32 key_size;
    U8 key[5];
    Utility_Get32BitUnsignedValueFromBuffer(request_bytes.message, 0, &message_type);
    Utility_Get32BitUnsignedValueFromBuffer(request_bytes.message, 4, &key_size);
    Utility_GetStringFromBuffer(request_bytes.message, 8, 5, &key);

    assert(message_size == request_bytes.size);
    assert(message_type == MESSAGE_TYPE_GET_VALUE_REQUEST);
    assert(key_size == 5);
    assert(Utility_AreTwoBuffersTheSame(key, "hello", 5));

    free(request_bytes.message);
}

void get_value_request_verify_serialized_data_is_deserialized_correctly()
{
    PP_PRINT_TEST();

    GetValueRequest request = { .key = { .message = (U8*)"hello", .size = 5 } };
    SizedMessage request_bytes;
    GetValueRequest_Serialize(&request, &request_bytes);

    GetValueRequest deserialized_request;
    GetValueRequest_Deserialize(&request_bytes, &deserialized_request);

    assert(Utility_AreTwoBuffersTheSame(request.key.message, deserialized_request.key.message, request.key.size));

    free(request_bytes.message);
    free(deserialized_request.key.message);
}

int main()
{
    printf("STARTING TESTS\n");
    
    QueueTest_InitializeAndDestroy();
    QueueTest_IsEmpty();
    QueueTest_PushBackSingle();
    QueueTest_PushBackTwo();

    get_value_request_verify_serialized_data_is_correct();
    get_value_request_verify_serialized_data_is_deserialized_correctly();
    // testThreadPool();
}
