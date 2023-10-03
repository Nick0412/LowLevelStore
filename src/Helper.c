#include "Helper.h"

IPv4Address IPv4Address_Create(U8 byte_0, U8 byte_1, U8 byte_2, U8 byte_3)
{
    IPv4Address address = {
        .ip_parts = {byte_0, byte_1, byte_2, byte_3}
    };

    return address;
}

ServerConnectionDetails ServerConnectionDetails_Create(const IPv4Address* ip_address, Port port, U32 number_of_connections_allowed)
{
    ServerConnectionDetails details = {
        .ip_address = *ip_address,
        .number_of_connections_allowed = number_of_connections_allowed,
        .port = port,
        .server_socket = socket(AF_INET, SOCK_STREAM, 0),
        .server_socket_address = SocketAddress_Create(ip_address, port)
    };

    return details;
}

void Vector_Initialize(
    Vector* vector,
    U32 number_of_elements,
    U32 capacity,
    U32 size_of_element_type)
{
    *vector = (Vector) {
        .capacity = capacity,
        .number_of_elements = number_of_elements,
        .size_of_element_type = size_of_element_type,
        .data_pointer = malloc(capacity * size_of_element_type)
    };
}

void Vector_Destroy(Vector* vector)
{
    free(vector->data_pointer);

    *vector = (Vector) {
        .capacity = 0,
        .number_of_elements = 0,
        .size_of_element_type = 0,
        .data_pointer = NULL
    };
}

void Vector_CopyElementAtIndex(Vector* vector, U32 index, void* location_to_copy_element)
{
    if (index > vector->number_of_elements)
    {
        perror("Index for vector is out of range.\n");
    }

    U32 byte_offset = vector->size_of_element_type * index;
    void* byte_offset_address = (U8*)vector->data_pointer + byte_offset;

    memcpy(location_to_copy_element, byte_offset_address, vector->size_of_element_type);
}

void Vector_AddElementToBack(Vector* vector, void* element_address)
{
    if (vector->number_of_elements == vector->capacity)
    {
        Vector_Internal_Resize(vector);
    }

    U32 current_byte_offset = vector->number_of_elements * vector->size_of_element_type;
    void* current_byte_offset_address = (U8*)vector->data_pointer + current_byte_offset;

    memcpy(current_byte_offset_address, element_address, vector->size_of_element_type);
    vector->number_of_elements++;
}

// TODO: Finish implementation.
void Vector_Internal_Resize(Vector* vector)
{
    U32 new_vector_capacity = 2 * vector->capacity;
    void* new_vector_data = malloc(new_vector_capacity * vector->size_of_element_type);

    memcpy(
        new_vector_data,
        vector->data_pointer,
        vector->number_of_elements * vector->size_of_element_type);
    
}

// TODO: Incorporate retries into logic.
void Socket_SendBytesWithRetries(
    Socket file_descriptor,
    U8* buffer_to_send,
    U32 buffer_size,
    U32 number_of_retries)
{
    (void)number_of_retries;
    U32 total_number_of_bytes_sent = 0;
    U32 number_of_bytes_remaining = buffer_size;
    
    while (number_of_bytes_remaining > 0)
    {
        number_of_bytes_remaining = buffer_size - total_number_of_bytes_sent;
        int bytes_sent_this_iteration = send(
            file_descriptor,
            buffer_to_send + total_number_of_bytes_sent,
            number_of_bytes_remaining,
            0);
        total_number_of_bytes_sent += bytes_sent_this_iteration;

        if (bytes_sent_this_iteration < 0)
        {
            perror("Socket_SendBytesWithRetries failed to send bytes");
        }
    }
}

void Socket_ReceiveBytesWithRetries(
    Socket file_descriptor,
    U8* buffer_to_receive,
    U32 buffer_size,
    U32 number_of_retries)
{
    (void)number_of_retries;
    U32 total_number_of_bytes_received = 0;
    U32 number_of_bytes_remaining = buffer_size;

    while (number_of_bytes_remaining > 0)
    {
        number_of_bytes_remaining = buffer_size - total_number_of_bytes_received;
        int bytes_received_this_iteration = recv(
            file_descriptor,
            buffer_to_receive + total_number_of_bytes_received,
            number_of_bytes_remaining,
            0);
        total_number_of_bytes_received += bytes_received_this_iteration;

        if (bytes_received_this_iteration < 0)
        {
            perror("Socket_ReceiveBytesWithRetries failed to receive bytes");
        }
    }
}

String IPv4Address_Internal_ConvertToString_Allocation(const IPv4Address* ip_address)
{
    U32 number_of_characters_in_ip_address = 0;
    // Need 3 characters for the decimals
    number_of_characters_in_ip_address += 3;
    for (U32 i = 0; i < 4; i++)
    {
        number_of_characters_in_ip_address += Utility_CountDigitsInUnsigned16Value(ip_address->ip_parts[i]);
    }

    String string_data = malloc(number_of_characters_in_ip_address);
    IPv4Address_Internal_WriteIPv4AddressToBuffer(ip_address, string_data);

    return string_data;
}

void IPv4Address_Internal_WriteIPv4AddressToBuffer(const IPv4Address* ip_address, String buffer_to_write_to)
{
    U32 offset = 0;
    for (U32 i = 0; i < 4; i++)
    {
        offset += sprintf(buffer_to_write_to + offset, "%d", ip_address->ip_parts[i]);
        if (i != 3)
        {
            offset += sprintf(buffer_to_write_to + offset, ".");
        }
    }
}

SocketAddress SocketAddress_Create(const IPv4Address* ip_address, U16 port)
{
    SocketAddress address = {
        .sin_family = AF_INET,
        .sin_port = htons(port)
    };

    char* ip_address_string = IPv4Address_Internal_ConvertToString_Allocation(ip_address);
    inet_pton(AF_INET, ip_address_string, &address.sin_addr);
    free(ip_address_string);

    return address;
}

void Socket_ConnectClientSocketToAddress(Socket client_socket, SocketAddress address)
{
    S32 result = connect(client_socket, (GenericSocketAddress*)&address, sizeof(address));
    if (result < 0)
    {
        perror("Connect failed\n");
    }
}

void Socket_BindAddressToServerSocket(Socket server_socket, SocketAddress* address)
{
    S32 result = bind(server_socket, (GenericSocketAddress*)address, sizeof(SocketAddress));
    if (result < 0)
    {
        perror("Bind failed\n");
    }
}

void Socket_ListenOnServerSocket(Socket server_socket, U32 number_of_connections)
{
    S32 result = listen(server_socket, number_of_connections);
    if (result < 0)
    {
        perror("Listen failed\n");
    }
}

void PollCollection_Initialize(PollCollection* poll_collection, Socket server_socket, U32 initial_capacity)
{
    *poll_collection = (PollCollection) {
        .capacity = initial_capacity,
        .index_of_listener_socket = 0,
        .number_of_elements = 1,
        .poll_fds = malloc(initial_capacity * sizeof(PollFileDescriptor))
    };

    poll_collection->poll_fds[0] = (PollFileDescriptor) {
        .fd = server_socket,
        .events = POLLIN
    };
}

void PollCollection_AddNewConnection(PollCollection* poll_collection, Socket client_socket)
{
    if (poll_collection->number_of_elements == poll_collection->capacity)
    {
        PollCollection_Internal_Resize(poll_collection);
    }

    poll_collection->poll_fds[poll_collection->number_of_elements] = (PollFileDescriptor) {
        .fd = client_socket,
        .events = POLLIN
    };

    poll_collection->number_of_elements++;
}

// TODO: Finish
void PollCollection_Poll(PollCollection* poll_collection, S32 timeout_in_millis, ThreadPool* thread_pool)
{
    S32 poll_result = poll(poll_collection->poll_fds, poll_collection->number_of_elements, timeout_in_millis);
    PollCollection_Internal_HandlePollErrors(poll_result);

    for (U32 index = 0; index < poll_collection->number_of_elements; index++)
    {
        PollFileDescriptor poll_fd = poll_collection->poll_fds[index];
        if (poll_fd.revents & POLLIN)
        {
            if (index == poll_collection->index_of_listener_socket)
            {
                Socket listener_socket = poll_fd.fd;
                PollCollection_Internal_HandleListenerSocket(poll_collection, listener_socket);
            }
            else
            {
                PollCollection_Internal_HandleDataSocket(thread_pool, poll_fd.fd);
            }
        }
    }
}

void PollCollection_Internal_HandlePollErrors(S32 poll_result)
{
    if (poll_result < 0)
    {
        perror("Poll encountered an error\n");
    }

    if (poll_result == 0)
    {
        perror("Poll timed out\n");
    }
}

void PollCollection_Internal_Resize(PollCollection* poll_collection)
{
    U32 new_capacity = poll_collection->capacity * 2;
    void* new_fds_pointer = malloc(new_capacity * sizeof(PollFileDescriptor));
    memcpy(
        new_fds_pointer,
        poll_collection->poll_fds,
        poll_collection->capacity * sizeof(PollFileDescriptor));
    free(poll_collection->poll_fds);

    *poll_collection =  (PollCollection) {
        .capacity = new_capacity,
        .index_of_listener_socket = poll_collection->index_of_listener_socket,
        .number_of_elements = poll_collection->number_of_elements,
        .poll_fds = new_fds_pointer
    };
}

void PollCollection_Internal_HandleListenerSocket(PollCollection* poll_collection, Socket listening_socket)
{
    SocketAddress client_socket_address;
    SocketSize client_socket_size;
    Socket new_socket = accept(
        listening_socket,
        (GenericSocketAddress*)&client_socket_address,
        &client_socket_size);

    PollCollection_AddNewConnection(poll_collection, new_socket);
}

void PollCollection_Internal_HandleDataSocket(ThreadPool* thread_pool, Socket client_connected_socket)
{
    ThreadTask task = {
        .argument = (void*)&client_connected_socket,
        .argument_size = sizeof(Socket),
        .function = NULL
    };

    ThreadPool_AddWork(thread_pool, &task);
}

void Queue_Initialize(Queue* queue)
{
    *queue = (Queue) {
        .head = NULL,
        .number_of_elements = 0,
        .tail = NULL
    };
}

void Queue_Destroy(Queue* queue)
{
    if (queue->head == NULL)
    {
        return;
    }

    else if (queue->head == queue->tail)
    {
        Queue_Internal_OneNodeDestroy(queue);
        return;
    }

    else
    {
        Queue_Internal_MultiNodeDestroy(queue);
    }
}

void Queue_IsEmpty(Queue* queue, bool* return_queue_is_empty)
{
    *return_queue_is_empty = (queue->number_of_elements == 0);
}

void Queue_PushBack(Queue* queue, void* data, U32 number_of_bytes)
{
    QueueNode* new_node = malloc(sizeof(QueueNode));
    *new_node = (QueueNode) {
        .next = NULL,
        .data_pointer = malloc(number_of_bytes),
        .previous = queue->tail,
        .number_of_bytes = number_of_bytes
    };
    memcpy(new_node->data_pointer, data, number_of_bytes);

    if (queue->head == NULL)
    {
        queue->head = new_node;
        queue->tail = new_node;
    }

    else
    {
        queue->tail->next = new_node;
        queue->tail = new_node;
    }

    queue->number_of_elements++;
}

void Queue_PushBackWithoutCopy(Queue* queue, void* data, U32 number_of_bytes)
{
    QueueNode* new_node = malloc(sizeof(QueueNode));
    *new_node = (QueueNode) {
        .next = NULL,
        .data_pointer = data,
        .previous = queue->tail,
        .number_of_bytes = number_of_bytes
    };

    if (queue->number_of_elements == 0)
    {
        queue->head = new_node;
        queue->tail = new_node;
    }
    else
    {
        queue->tail->next = new_node;
        queue->tail = new_node;
    }

    queue->number_of_elements++;
}

void Queue_GetSizeOfHead(Queue* queue, U32* return_number_of_bytes)
{
    if (queue == NULL)
    {
        perror("Queue is NULL\n");
    }

    if (queue->number_of_elements != 0)
    {
        *return_number_of_bytes = queue->head->number_of_bytes;
    }
    else
    {
        *return_number_of_bytes = 0;
    }
}

void Queue_PopHead(Queue* queue, void* return_data_pointer)
{
    if (return_data_pointer == NULL)
    {
        perror("Return data pointer is NULL");
        return;
    }

    if (queue->number_of_elements == 0)
    {
        perror("Tried to remove from empty queue");
        return;
    }

    if (queue->number_of_elements == 1)
    {
        memcpy(return_data_pointer, queue->head->data_pointer, queue->head->number_of_bytes);
        free(queue->head->data_pointer);
        free(queue->head);

        queue->head = NULL;
        queue->tail = NULL;
        queue->number_of_elements = 0;
        return;
    }

    QueueNode* node = queue->head;
    queue->head = queue->head->next;
    queue->head->previous = NULL;
    queue->number_of_elements--;

    memcpy(return_data_pointer, node->data_pointer, node->number_of_bytes);
    free(node->data_pointer);
    free(node);
    node = NULL;
}

bool Queue_Internal_IsEmpty(Queue* queue)
{
    bool is_empty;
    Queue_IsEmpty(queue, &is_empty);

    return is_empty;
}

void Queue_Internal_OneNodeDestroy(Queue* queue)
{
    QueueNode* node = queue->head;
    free(node->data_pointer);
    free(node);

    *queue = (Queue) {
        .head = NULL,
        .tail = NULL,
        .number_of_elements = 0
    };
}

void Queue_Internal_MultiNodeDestroy(Queue* queue)
{
    QueueNode* current = queue->head;
    QueueNode* next = current->next;
    
    while (current != NULL)
    {
        free(current->data_pointer);
        free(current);

        current = next;
        next = (next == NULL ? NULL : next->next);
    }

    *queue = (Queue) {
        .head = NULL,
        .tail = NULL,
        .number_of_elements = 0
    };
}

void ThreadPool_Initialize(ThreadPool* thread_pool, U32 number_of_threads)
{
    thread_pool->number_of_threads = number_of_threads;
    thread_pool->worker_threads = calloc(number_of_threads, sizeof(Thread));
    thread_pool->should_stop = false;

    Queue_Initialize(&thread_pool->task_queue);
    pthread_mutex_init(&thread_pool->queue_mutex, NULL);
    pthread_cond_init(&thread_pool->work_is_ready_cv, NULL);

    for (U32 i = 0; i < number_of_threads; i++)
    {
        pthread_create(&thread_pool->worker_threads[i], NULL, &ThreadPool_Internal_ThreadFunction, thread_pool);
    }
}

void ThreadPool_AddWork(ThreadPool* thread_pool, const ThreadTask* thread_task)
{
    pthread_mutex_lock(&thread_pool->queue_mutex);

    if (!thread_pool->should_stop)
    {
        // Setup the thread task data and argument.
        U32 size_of_task_and_argument = sizeof(ThreadTask) + thread_task->argument_size;
        void* task_data = malloc(size_of_task_and_argument);
        void* argument_offset_pointer = (U8*)task_data + sizeof(ThreadTask);
        memcpy(task_data, thread_task, sizeof(ThreadTask));
        memcpy(thread_task->argument, argument_offset_pointer, thread_task->argument_size);
        ThreadPool_Internal_FixThreadTaskArgumentPointer(task_data);

        // Place that data on the queue and alert any waiting threads there is data
        Queue_PushBackWithoutCopy(&thread_pool->task_queue, task_data, size_of_task_and_argument);
        pthread_cond_broadcast(&thread_pool->work_is_ready_cv);
    }

    pthread_mutex_unlock(&thread_pool->queue_mutex);
}

void ThreadPool_Internal_FixThreadTaskArgumentPointer(void* encoded_thread_task_with_argument)
{
    ThreadTask* task_pointer = (ThreadTask*)encoded_thread_task_with_argument;
    void* argument_pointer = (U8*)encoded_thread_task_with_argument + sizeof(ThreadTask);
    task_pointer->argument = argument_pointer;
}

void* ThreadPool_Internal_ThreadFunction(void* thread_argument)
{
    ThreadPool* thread_pool = (ThreadPool*)thread_argument;

    while (true)
    {
        pthread_mutex_lock(&thread_pool->queue_mutex);
        while (!thread_pool->should_stop && Queue_Internal_IsEmpty(&thread_pool->task_queue))
        {
            pthread_cond_wait(&thread_pool->work_is_ready_cv, &thread_pool->queue_mutex);
        }

        if (thread_pool->should_stop)
        {
            break;
        }

        U32 thread_task_and_argument_size;
        Queue_GetSizeOfHead(&thread_pool->task_queue, &thread_task_and_argument_size);
        void* thread_task_and_argument = malloc(thread_task_and_argument_size);

        Queue_PopHead(&thread_pool->task_queue, thread_task_and_argument);
        ThreadPool_Internal_FixThreadTaskArgumentPointer(thread_task_and_argument);

        pthread_mutex_unlock(&thread_pool->queue_mutex);

        ThreadTask* thread_task = (ThreadTask*)thread_task_and_argument;
        thread_task->function(thread_task->argument);

        free(thread_task_and_argument);
    }

    pthread_mutex_unlock(&thread_pool->queue_mutex);
    return NULL;
}

void ThreadPool_Destroy(ThreadPool* thread_pool)
{
    pthread_mutex_lock(&thread_pool->queue_mutex);
    Queue_Destroy(&thread_pool->task_queue);
    thread_pool->should_stop = true;
    pthread_cond_broadcast(&thread_pool->work_is_ready_cv);
    pthread_mutex_unlock(&thread_pool->queue_mutex);

    for (U32 i = 0; i < thread_pool->number_of_threads; i++)
    {
        pthread_join(thread_pool->worker_threads[i], NULL);
    }

    free(thread_pool->worker_threads);
}

// void Messages_HandleMessage(Socket client_connected_socket)
// {
//     // Recv 4 bytes to get size
    
//     // Malloc buffer to hold remaining data
//     // Get message type
//     // Dispatch based on type
//     // Deserialize data into message struct 
//     // Do action
//     // Get Result 
//     // Serialize Result
//     // Send Result back
// }

void Utility_PrintHexBytes(void* data, U32 number_of_bytes)
{
    for (U32 i = 0; i < number_of_bytes; i++)
    {
        printf("0x%x ", ((U8*)data)[i]);
    }

    printf("\n");
}

bool Utility_AreBytesTheSame(void* data1, void* data2, U32 number_of_bytes)
{
    return (memcmp(data1, data2, number_of_bytes) == 0);
}

void Utility_Get32BitUnsignedValueFromBuffer(const void* source_buffer, U32 offset_into_source, U32* return_value)
{
    U8* byte_buffer = (U8*)source_buffer;
    *return_value = (byte_buffer[offset_into_source + 3] & 0xFF) << 24 |
                    (byte_buffer[offset_into_source + 2] & 0xFF) << 16 |
                    (byte_buffer[offset_into_source + 1] & 0xFF) << 8 |
                    (byte_buffer[offset_into_source] & 0xFF);

    *return_value = ntohl(*return_value);
}

void Utility_Set32BitUnsignedValueInBuffer(void* destination_buffer, U32 offset_into_source, const U32 value)
{
    U8* byte_buffer = (U8*)destination_buffer;
    U32 network_value = htonl(value);

    byte_buffer[offset_into_source] = network_value & 0xFF;
    byte_buffer[offset_into_source + 1] = (network_value >> 8) & 0xFF;
    byte_buffer[offset_into_source + 2] = (network_value >> 16) & 0xFF;
    byte_buffer[offset_into_source + 3] = (network_value >> 24) & 0xFF;
}

void Utility_GetStringFromBuffer(const void* source_buffer, const U32 offset_into_buffer, const U32 string_size, void* return_string_buffer)
{
    memcpy(return_string_buffer, source_buffer + offset_into_buffer, string_size);
}

void Utility_SetStringInBuffer(void* destination_buffer, const U32 offset_into_buffer, const U32 string_size, const void* source_string_buffer)
{
    memcpy(destination_buffer + offset_into_buffer, source_string_buffer, string_size);
}

U32 Utility_CountDigitsInUnsigned16Value(U16 number)
{
    return log10(number) + 1;
}