// #ifndef CLIENT_H
// #define CLIENT_H

// #include "messages/GetValueMessageRequest.h"
// #include "messages/GetValueMessageResponse.h"
// #include "messages/PutKeyValueMessageRequest.h"
// #include "messages/PutKeyValueMessageResponse.h"

// #include <stdint.h>
// #include <pthread.h>


// typedef struct ClientFunctions
// {
//     void (*callGetValueMessage)(pthread_t thread, const GetValueMessageRequest* request);
//     void (*callPutKeyValueMessage)(pthread_t thread, const PutKeyValueMessageRequest* request);

//     // All responses are allocated in the functions. No need to preallocate.
//     void (*joinGetValueMessage)(pthread_t thread, GetValueMessageResponse* response);
//     void (*joinPutKeyValueMessage)(pthread_t thread, PutKeyValueMessageResponse* response);

// } ClientFunctions;

// /**
//  * ip_address is of the form ###.###.###.###
// */
// typedef struct Client
// {
//     uint16_t port;
//     char ip_address[15];
//     int client_socket;
//     ClientFunctions functions;

// } Client;

// void callGetValueMessage(pthread_t thread, const GetValueMessageRequest* request);

// void callPutKeyValueMessage(pthread_t thread, const PutKeyValueMessageRequest* request);

// void joinGetValueMessage(pthread_t thread, GetValueMessageResponse* response);

// void joinPutKeyValueMessage(pthread_t thread, PutKeyValueMessageResponse* response);

// #endif
