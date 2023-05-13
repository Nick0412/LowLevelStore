#ifndef IN_MEMORY_DATA_STORE_H
#define IN_MEMORY_DATA_STORE_H

#include "common/StringBuffer.h"
#include "KeyValueEntity.h"
#include <stdint.h>

/**
 * Naive memory store. For now we will only support insertions and lookups.
 * For now we will exclude the memory pool usage and use a direct destroy function.
 */
typedef struct InMemoryDataStore
{
    KeyValueEntity* pairs;
    uint32_t offset;
    uint32_t size;

} InMemoryDataStore;

void initializeInMemoryDataStore(InMemoryDataStore* data_store, uint32_t size);

StringBuffer* getValue(InMemoryDataStore* data_store, StringBuffer* key);

void putKeyValuePair(InMemoryDataStore* data_store, KeyValueEntity* entity);

void destroyInMemoryDataStore(InMemoryDataStore* data_store);

#endif
