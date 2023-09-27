#include "Helper.h"

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

SocketAddress Socket_CreateIPv4InetAddress(char* ip_address, U16 port)
{
    SocketAddress address = {
        .sin_family = AF_INET,
        .sin_port = htons(port)
    };
    inet_aton(ip_address, &address.sin_addr);

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

// TODO: Finish
void PollCollection_AddNewConnection(PollCollection* poll_collection, Socket client_socket)
{
    if (poll_collection->number_of_elements)
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
void PollCollection_Poll(PollCollection* poll_collection, S32 timeout_in_millis)
{
    S32 poll_result = poll(poll_collection->poll_fds, poll_collection->number_of_elements, timeout_in_millis);

    if (poll_result < 0)
    {
        perror("Poll encountered an error\n");
    }

    if (poll_result == 0)
    {
        perror("Poll timed out\n");
    }

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
                // PollCollection_Internal_HandleDataSocket();
            }
        }
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

// void PollCollection_Internal_HandleDataSocket();

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

    if (queue->head == queue->tail)
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
    if (queue->head != NULL)
    {
        *return_number_of_bytes = queue->head->number_of_bytes;
    }
}

void Queue_PopHead(Queue* queue, void* return_data_pointer)
{
    QueueNode* node = queue->head;
    memcpy(return_data_pointer, node->data_pointer, node->number_of_bytes);

    if (queue->head == queue->tail)
    {
        free(queue->head->data_pointer);
        free(queue->head);

        *queue = (Queue) {
            .head = NULL,
            .tail = NULL,
            .number_of_elements = 0
        };
    }

    else
    {
        free(node->data_pointer);
        free(node);
        queue->head = queue->head->next;
        queue->number_of_elements--;
    }
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
        next = next->next;
    }

    *queue = (Queue) {
        .head = NULL,
        .tail = NULL,
        .number_of_elements = 0
    };
}

