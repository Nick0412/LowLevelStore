#include "datastore/InMemoryDataStore.h"
#include <stdlib.h>
#include <stdio.h>

void initializeInMemoryDataStore(InMemoryDataStore* data_store, uint32_t size)
{
    data_store->size = size;
    data_store->offset = 0;
    data_store->pairs = malloc(sizeof(KeyValueEntity)*size);
}

StringBuffer* getValue(InMemoryDataStore* data_store, StringBuffer* key)
{
    for (uint32_t i = 0; i < data_store->size; i++)
    {   
        StringBuffer* current_key = data_store->pairs[i].key;
        if (areStringBuffersEqual(key, current_key))
        {
            return data_store->pairs[i].value;
        }
    }

    return NULL;
}

void putKeyValuePair(InMemoryDataStore* data_store, KeyValueEntity* entity)
{
    data_store->pairs[data_store->offset] = *entity;
    data_store->offset++;
}

void destroyInMemoryDataStore(InMemoryDataStore* data_store)
{
    free(data_store->pairs);
    data_store->pairs = NULL;
    data_store->offset = 0;
    data_store->size = 0;
}
