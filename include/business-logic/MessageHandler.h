#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "common/SizeAwareBuffer.h"
#include "datastore/InMemoryDataStore.h"
#include "network-utils/TcpFunctions.h"

void handleMessage(SizeAwareBuffer* incoming_message, InMemoryDataStore* store, int socket_pointer);

void handlePutKeyValueMessageRequest(SizeAwareBuffer* incoming_message, InMemoryDataStore* store, int socket_pointer);

void handleGetValueMessageRequest(SizeAwareBuffer* incoming_message, InMemoryDataStore* store, int socket_pointer);

#endif
