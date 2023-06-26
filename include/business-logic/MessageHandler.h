#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "common/BufferHelper.h"
#include "datastore/InMemoryDataStore.h"
#include "network-utils/TcpFunctions.h"

void handleMessage(AugmentedBuffer* incoming_message, InMemoryDataStore* store, int socket_pointer);

void handlePutKeyValueMessageRequest(AugmentedBuffer* incoming_message, InMemoryDataStore* store, int socket_pointer);

void handleGetValueMessageRequest(AugmentedBuffer* incoming_message, InMemoryDataStore* store, int socket_pointer);

#endif
