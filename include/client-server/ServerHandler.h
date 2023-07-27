#ifndef SERVER_HANDLER_H
#define SERVER_HANDLER_H

#include "common/SizeAwareBuffer.h"
#include "datastore/InMemoryDataStore.h"
#include "messages/MessageTypes.h"
#include "messages/GetValueMessageRequest.h"
#include "messages/PutKeyValueMessageRequest.h"

#include <stdint.h>

void ServerHandler_HandleMessage(int socket, InMemoryDataStore* store);

void ServerHandler_HandleGetValueMessage(int socket, InMemoryDataStore* store,
    const GetValueMessageRequest* request);

void ServerHandler_HandlePutKeyValueMessage(int socket, InMemoryDataStore* store,
    const PutKeyValueMessageRequest* request);

#endif
