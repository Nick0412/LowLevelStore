#ifndef HELPER_H
#define HELPER_H

#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef int8_t S8;
typedef int16_t S16;
typedef int32_t S32;
typedef int64_t S64;

typedef int Socket;
typedef struct sockaddr_in SocketAddress;
typedef socklen_t SocketSize;
typedef struct sockaddr GenericSocketAddress;
typedef struct pollfd PollFileDescriptor;

typedef struct Vector
{
    U32 number_of_elements;
    U32 capacity;
    U32 size_of_element_type;
    void* data_pointer;
} Vector;

typedef struct QueueNode
{
    struct QueueNode* next;
    struct QueueNode* previous;
    U32 number_of_bytes;
    void* data_pointer;
} QueueNode;

typedef struct Queue
{
    QueueNode* head;
    QueueNode* tail;
    U32 number_of_elements;
} Queue;

typedef struct InMemoryKeyValueStore
{

} InMemoryKeyValueStore;

typedef struct PollCollection
{
    U32 number_of_elements;
    U32 capacity;
    U32 index_of_listener_socket;
    PollFileDescriptor* poll_fds;
} PollCollection;

typedef struct ThreadPool
{

} ThreadPool;

typedef struct KeyValueServer
{
    Socket server_socket;
    SocketAddress socket_address;
    InMemoryKeyValueStore datastore;
    PollCollection poll_collection;
    ThreadPool thread_pool;
} KeyValueServer;

void Vector_Initialize(Vector* vector, U32 number_of_elements, U32 capacity, U32 size_of_element_type);
void Vector_Destroy(Vector* vector);
void Vector_CopyElementAtIndex(Vector* vector, U32 index, void* location_to_copy_element);
void Vector_AddElementToBack(Vector* vector, void* element_address);
void Vector_Internal_Resize(Vector* vector);

void Socket_SendBytesWithRetries(Socket file_descriptor, U8* buffer_to_send, U32 buffer_size, U32 number_of_retries);
void Socket_ReceiveBytesWithRetries(Socket file_descriptor, U8* buffer_to_receive, U32 buffer_size, U32 number_of_retries);
SocketAddress Socket_CreateIPv4InetAddress(char* ip_address, U16 port);
void Socket_ConnectClientSocketToAddress(Socket client_socket, SocketAddress address);
void Socket_BindAddressToServerSocket(Socket server_socket, SocketAddress* address);
void Socket_ListenOnServerSocket(Socket server_socket, U32 number_of_connections);

void PollCollection_Initialize(PollCollection* poll_collection, Socket server_socket, U32 number_of_elements);
void PollCollection_AddNewConnection(PollCollection* poll_collection, Socket client_socket);
void PollCollection_Poll(PollCollection* poll_collection, S32 timeout_in_millis);
void PollCollection_Internal_Resize(PollCollection* poll_collection);
void PollCollection_Internal_HandleListenerSocket(PollCollection* poll_collection, Socket listening_socket);
void PollCollection_Internal_HandleDataSocket();

void Queue_Initialize(Queue* queue);
void Queue_Destroy(Queue* queue);
void Queue_IsEmpty(Queue* queue, bool* return_queue_is_empty);
void Queue_PushBack(Queue* queue, void* data, U32 number_of_bytes);
void Queue_GetSizeOfHead(Queue* queue, U32* return_number_of_bytes);
void Queue_PopHead(Queue* queue, void* return_data_pointer);
void Queue_Internal_OneNodeDestroy(Queue* queue);
void Queue_Internal_MultiNodeDestroy(Queue* queue);

#endif
