#ifndef HELPER_H
#define HELPER_H

#include <arpa/inet.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/socket.h>
#include <poll.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdatomic.h>
#include <netinet/in.h>
#include <math.h>

#define PRINT_TEST() printf("Starting test: %s\n", __func__);
#define DEBUG(num) printf("Point %d\n", num);

typedef char* String;

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef int8_t S8;
typedef int16_t S16;
typedef int32_t S32;
typedef int64_t S64;

typedef int Socket;
typedef U16 Port;
typedef struct sockaddr_in SocketAddress;
typedef socklen_t SocketSize;
typedef struct sockaddr GenericSocketAddress;
typedef struct pollfd PollFileDescriptor;

typedef pthread_t Thread;
typedef pthread_mutex_t Mutex;
typedef pthread_cond_t ConditionVariable;
typedef void (*RunFunction)(void*);

typedef struct Vector
{
    U32 number_of_elements;
    U32 capacity;
    U32 size_of_element_type;
    void* data_pointer;
} Vector;

typedef struct SizedMessage
{
    U32 size;
    U8* message;
} SizedMessage;

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
    SizedMessage keys[100];
    SizedMessage values[100];
    U32 current_index;
} InMemoryKeyValueStore;

typedef struct PollCollection
{
    U32 number_of_elements;
    U32 capacity;
    U32 index_of_listener_socket;
    PollFileDescriptor* poll_fds;
} PollCollection;

typedef struct ThreadTask
{
    RunFunction function;
    U32 argument_size;
    void* argument;
} ThreadTask;

typedef struct ThreadPool
{
    Mutex queue_mutex;
    ConditionVariable work_is_ready_cv;
    Queue task_queue;
    U32 number_of_threads;
    bool should_stop;
    Thread* worker_threads;
} ThreadPool;

typedef struct SocketThreadGroup
{
    Socket listening_socket;
    atomic_bool should_close;
    Thread* thread_group;
} SocketThreadGroup;

typedef struct ThreadWorkerArguments
{
    Socket client_socket;
    InMemoryKeyValueStore* datastore;
} ThreadWorkerArguments;

typedef enum MessageStatus
{
    MESSAGE_STATUS_SUCCESS = 1,
    MESSAGE_STATUS_FAILURE = 2
} MessageStatus;

typedef enum MessageType
{
    MESSAGE_TYPE_PUT_KEY_VALUE_REQUEST = 1,
    MESSAGE_TYPE_PUT_KEY_VALUE_RESPONSE = 2,
    MESSAGE_TYPE_PUT_KEY_VALUE_ERROR = 3,

    MESSAGE_TYPE_GET_VALUE_REQUEST = 10,
    MESSAGE_TYPE_GET_VALUE_RESPONSE = 11,
    MESSAGE_TYPE_GET_VALUE_ERROR = 12
} MessageType;

typedef enum MessageOffset
{
    MESSAGE_OFFSET_SIZE = 0,
    MESSAGE_OFFSET_TYPE = 4,
    MESSAGE_OFFSET_DATA = 8
} MessageOffset;

/**
 * Struct representation of an IP address. `ip_parts[0]` is meant to be the most significant byte and
 * `ip_parts[3]` is the least.
 * 
 * Ex. IP Address 1.2.3.4 would be represented as
 * ip_parts[0] = 1
 * ip_parts[1] = 2
 * ip_parts[2] = 3
 * ip_parts[3] = 4
*/
typedef struct IPv4Address
{
    U8 ip_parts[4];
} IPv4Address;

typedef struct ServerConnectionDetails
{
    Socket server_socket;
    SocketAddress server_socket_address;
    IPv4Address ip_address;
    Port port;
    U32 number_of_connections_allowed;
} ServerConnectionDetails;

typedef struct LowLevelStoreServer
{
    ServerConnectionDetails connection_details;
    InMemoryKeyValueStore datastore;
    PollCollection poll_collection;
    ThreadPool thread_pool;
} LowLevelStoreServer;

/**
 **************************************************************************************************
 ************************************** FUNCTION DECLARATIONS *************************************
 **************************************************************************************************
*/

IPv4Address IPv4Address_Create(U8 byte_0, U8 byte_1, U8 byte_2, U8 byte_3);
// Allocates space for string. Needs to be cleaned up.
String IPv4Address_Internal_ConvertToString_Allocation(const IPv4Address* ip_address);
void IPv4Address_Internal_WriteIPv4AddressToBuffer(const IPv4Address* ip_address, String buffer_to_write_to);
ServerConnectionDetails ServerConnectionDetails_Create(const IPv4Address* ip_address, Port port, U32 number_of_connections_allowed);
SocketAddress SocketAddress_Create(const IPv4Address* address, U16 port);

void Vector_Initialize(Vector* vector, U32 number_of_elements, U32 capacity, U32 size_of_element_type);
void Vector_Destroy(Vector* vector);
void Vector_CopyElementAtIndex(Vector* vector, U32 index, void* location_to_copy_element);
void Vector_AddElementToBack(Vector* vector, void* element_address);
void Vector_Internal_Resize(Vector* vector);

void Socket_SendBytesWithRetries(Socket file_descriptor, U8* buffer_to_send, U32 buffer_size, U32 number_of_retries);
void Socket_ReceiveBytesWithRetries(Socket file_descriptor, U8* buffer_to_receive, U32 buffer_size, U32 number_of_retries);
void Socket_ConnectClientSocketToAddress(Socket client_socket, SocketAddress address);
void Socket_BindAddressToServerSocket(Socket server_socket, SocketAddress* address);
void Socket_ListenOnServerSocket(Socket server_socket, U32 number_of_connections);

void PollCollection_Initialize(PollCollection* poll_collection, Socket server_socket, U32 number_of_elements);
void PollCollection_AddNewConnection(PollCollection* poll_collection, Socket client_socket);
void PollCollection_Poll(PollCollection* poll_collection, S32 timeout_in_millis, ThreadPool* thread_pool);
void PollCollection_Internal_Resize(PollCollection* poll_collection);
void PollCollection_Internal_HandlePollErrors(S32 poll_result);
void PollCollection_Internal_HandleListenerSocket(PollCollection* poll_collection, Socket listening_socket);
void PollCollection_Internal_HandleDataSocket(ThreadPool* thread_pool, Socket client_connected_socket);

void Queue_Initialize(Queue* queue);
void Queue_Destroy(Queue* queue);
void Queue_IsEmpty(Queue* queue, bool* return_queue_is_empty);
void Queue_PushBack(Queue* queue, void* data, U32 number_of_bytes);
void Queue_PushBackWithoutCopy(Queue* queue, void* data, U32 number_of_bytes);
void Queue_GetSizeOfHead(Queue* queue, U32* return_number_of_bytes);
void Queue_PopHead(Queue* queue, void* return_data_pointer);
bool Queue_Internal_IsEmpty(Queue* queue);
void Queue_Internal_OneNodeDestroy(Queue* queue);
void Queue_Internal_MultiNodeDestroy(Queue* queue);

void ThreadPool_Initialize(ThreadPool* thread_pool, U32 number_of_threads);
void ThreadPool_AddWork(ThreadPool* thread_pool, const ThreadTask* thread_task);
void* ThreadPool_Internal_ThreadFunction(void* thread_argument);
void ThreadPool_Internal_FixThreadTaskArgumentPointer(void* encoded_thread_task_with_argument);
void ThreadPool_Destroy(ThreadPool* thread_pool);

void Utility_PrintHexBytes(void* data, U32 number_of_bytes);
bool Utility_AreBytesTheSame(void* data1, void* data2, U32 number_of_bytes);
void Utility_Get32BitUnsignedValueFromBuffer(const void* source_buffer, U32 offset_into_source, U32* return_value);
void Utility_Set32BitUnsignedValueInBuffer(void* destination_buffer, U32 offset_into_source, const U32 value);
void Utility_GetStringFromBuffer(const void* source_buffer, const U32 offset_into_buffer, const U32 string_size, void* return_string_buffer);
void Utility_SetStringInBuffer(void* destination_buffer, const U32 offset_into_buffer, const U32 string_size, const void* source_string_buffer);
U32 Utility_CountDigitsInUnsigned16Value(U16 number);
void Utility_CheckAndPrintForError(S32 result, S32 error_value_to_compare_against);
bool Utility_AreTwoBuffersTheSame(void* buffer1, void* buffer2, U32 number_of_bytes);

void* LowLevelStore_ThreadWorkerFunction(void* arguments);
void LowLevelStore_HandleMessage(const U8* message, const U32 message_size, InMemoryKeyValueStore* datastore, SizedMessage* return_message);

void InMemoryKeyValueStore_Initialize(InMemoryKeyValueStore* store);
void InMemoryKeyValueStore_PutKeyValue(InMemoryKeyValueStore* store, SizedMessage* key, SizedMessage* value);
void InMemoryKeyValueStore_GetValue_Allocation(const InMemoryKeyValueStore* store, SizedMessage* key, SizedMessage* return_value);
void InMemoryKeyValueStore_Destroy(InMemoryKeyValueStore* store);

#endif
