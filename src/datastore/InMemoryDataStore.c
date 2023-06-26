#include "datastore/InMemoryDataStore.h"
#include "common/MemoryPool.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void initializeInMemoryDataStore(InMemoryDataStore* store)
{
    store->current_size = 0;
    store->pool = malloc(sizeof(MemoryPoolList));
    initializeMemoryPool(store->pool);
}

void destroyInMemoryDataStore(InMemoryDataStore* store)
{
    destroyMemoryPool(store->pool);
    free(store->pool);
    store->pool = NULL;
}

void insertKeyValuePair(InMemoryDataStore* store, KeyValueEntity* kv_entity)
{
    AugmentedBuffer* test_if_value_exists;
    findValue(store, kv_entity->key, &test_if_value_exists);
    if (test_if_value_exists->buffer_size != -1)
    {
        return;
    }

    uint32_t block_size = 2 * sizeof(AugmentedBuffer) + kv_entity->key->buffer_size + kv_entity->value->buffer_size;
    AugmentedBuffer new_kv_buffer;
    allocateMemoryInPool(store->pool, block_size, &new_kv_buffer);

    AugmentedBuffer* first_ab = (AugmentedBuffer*)new_kv_buffer.buffer_pointer;
    first_ab->buffer_pointer = (char*)new_kv_buffer.buffer_pointer + (2 * sizeof(AugmentedBuffer));
    first_ab->buffer_size = kv_entity->key->buffer_size;

    AugmentedBuffer* second_ab = (AugmentedBuffer*)((char*)new_kv_buffer.buffer_pointer + sizeof(AugmentedBuffer));
    second_ab->buffer_pointer = (char*)new_kv_buffer.buffer_pointer + (2 * sizeof(AugmentedBuffer)) + kv_entity->key->buffer_size;
    second_ab->buffer_size = kv_entity->value->buffer_size;

    memcpy(first_ab->buffer_pointer, kv_entity->key->buffer_pointer, kv_entity->key->buffer_size);
    memcpy(second_ab->buffer_pointer, kv_entity->value->buffer_pointer, kv_entity->value->buffer_size);

    store->data[store->current_size].key = first_ab;
    store->data[store->current_size].value = second_ab;

    store->current_size++;
}

void findValue(InMemoryDataStore* store, const AugmentedBuffer* key, AugmentedBuffer** return_value)
{
    for (int i = 0; i < store->current_size; i++)
    {   
        if (areAugmentedBuffersSame(key, store->data[i].key))
        {
            *return_value = store->data[i].value;
            return;
        }
    }

    // TODO: Fix this uncleaned memory
    *return_value = malloc(sizeof(AugmentedBuffer));
    (*return_value)->buffer_pointer = NULL;
    (*return_value)->buffer_size = -1;
}
