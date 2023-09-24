#include "Header.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
