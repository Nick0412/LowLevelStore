#include "datastore/InMemoryDataStore.h"
#include "common/MemoryPool.h"
#include <string.h>
#include <stdio.h>

void initializeInMemoryDataStore(InMemoryDataStore* store)
{
    store->current_size = 0;
}

// TODO: Currently doesn't handle duplicate keys. Add check for this.
void insertKeyValuePair(MemoryPoolList* pool, InMemoryDataStore* store, KeyValueEntity* kv_entity)
{
    // Test if value is already in data store
    AugmentedBuffer test_value;
    findValue(pool, store, kv_entity->key, &test_value);
    if (test_value.buffer_size != -1)
    {
        return;
    }

    // Allocate both void pointers for the key and value
    AugmentedBuffer buffer_holding_key;
    AugmentedBuffer buffer_holding_value;
    allocateMemoryInPool(pool, kv_entity->key->buffer_size, &buffer_holding_key);
    allocateMemoryInPool(pool, kv_entity->value->buffer_size, &buffer_holding_value);

    // Copy key and values from kv_entity into the newly allocated void pointers
    memcpy(buffer_holding_key.buffer_pointer, kv_entity->key->buffer_pointer, kv_entity->key->buffer_size);
    memcpy(buffer_holding_value.buffer_pointer, kv_entity->value->buffer_pointer, kv_entity->value->buffer_size);

    // Allocated the actuated AugmentedBuffer structs to attach in the store
    AugmentedBuffer buffer_for_augmented_key;
    AugmentedBuffer buffer_for_augmented_value;
    allocateMemoryInPool(pool, sizeof(AugmentedBuffer), &buffer_for_augmented_key);
    allocateMemoryInPool(pool, sizeof(AugmentedBuffer), &buffer_for_augmented_value);

    // Set the allocated AugmentedBuffer fields to the correct values
    ((AugmentedBuffer*)buffer_for_augmented_key.buffer_pointer)->buffer_pointer = buffer_holding_key.buffer_pointer;
    ((AugmentedBuffer*)buffer_for_augmented_key.buffer_pointer)->buffer_size = buffer_holding_key.buffer_size;

    ((AugmentedBuffer*)buffer_for_augmented_value.buffer_pointer)->buffer_pointer = buffer_holding_value.buffer_pointer;
    ((AugmentedBuffer*)buffer_for_augmented_value.buffer_pointer)->buffer_size = buffer_holding_value.buffer_size;

    // Attach the allocated AugmentedBuffers inside the store
    store->data[store->current_size].key = (AugmentedBuffer*)buffer_for_augmented_key.buffer_pointer;
    store->data[store->current_size].value = (AugmentedBuffer*)buffer_for_augmented_value.buffer_pointer;

    // Increment current pointer into the store
    store->current_size++;
}

void findValue(MemoryPoolList* pool, const InMemoryDataStore* store, const AugmentedBuffer* key, AugmentedBuffer* return_value)
{
    for (int i = 0; i < store->current_size; i++)
    {
        if (areAugmentedBuffersSame(key, store->data[i].key))
        {
            // TODO: probably can allocate memory directly into return_value without using intermediate buffer
            AugmentedBuffer buff;
            uint32_t value_size = store->data[i].value->buffer_size;
            allocateMemoryInPool(pool, value_size, &buff);
            memcpy(buff.buffer_pointer, store->data[i].value->buffer_pointer, value_size);

            return_value->buffer_pointer = buff.buffer_pointer;
            return_value->buffer_size = buff.buffer_size;
            return;
        }
    }

    // TODO: Make these less sentinel value feeling.
    return_value->buffer_pointer = NULL;
    return_value->buffer_size = -1;
}
