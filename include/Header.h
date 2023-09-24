#ifndef HEADER_H
#define HEADER_H

#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <errno.h>

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
typedef struct sockaddr GenericSocketAddress;
typedef struct pollfd PollFileDescriptor;

typedef struct Vector
{
    U32 number_of_elements;
    U32 capacity;
    U32 size_of_element_type;
    void* data_pointer;
} Vector;

typedef struct InMemoryKeyValueStore
{

} InMemoryKeyValueStore;

typedef struct ServerSocket
{

} ServerSocket;

typedef struct KeyValueServer
{
    ServerSocket server_socket;
    InMemoryKeyValueStore datastore;

} KeyValueServer;

typedef struct PollCollection
{
    
} PollCollection;


void Vector_Initialize(
    Vector* vector,
    U32 number_of_elements,
    U32 capacity,
    U32 size_of_element_type);
void Vector_Destroy(Vector* vector);
void Vector_CopyElementAtIndex(Vector* vector, U32 index, void* location_to_copy_element);
void Vector_AddElementToBack(Vector* vector, void* element_address);
void Vector_Internal_Resize(Vector* vector);

void Socket_SendBytesWithRetries(
    Socket file_descriptor,
    U8* buffer_to_send,
    U32 buffer_size,
    U32 number_of_retries);
void Socket_ReceiveBytesWithRetries(
    Socket file_descriptor,
    U8* buffer_to_receive,
    U32 buffer_size,
    U32 number_of_retries);
SocketAddress Socket_CreateIPv4InetAddress(char* ip_address, U16 port);
void Socket_ConnectClientSocketToAddress(Socket client_socket, SocketAddress address);
void Socket_BindAddressToServerSocket(Socket server_socket, SocketAddress* address);
void Socket_ListenOnServerSocket(Socket server_socket, U32 number_of_connections);

// void PollCollection;

#endif
