#include "datastore/InMemoryDataStore.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void testInitializeInMemoryDataStore()
{
    InMemoryDataStore store;
    initializeInMemoryDataStore(&store);

    assert(store.current_size == 0);
}

void testKeyValueInsertion()
{
    InMemoryDataStore store;
    MemoryPoolList pool;
    
    initializeInMemoryDataStore(&store);
    initializeMemoryPool(&pool);

    KeyValueEntity kv = {
        .key = &(AugmentedBuffer) {
            .buffer_pointer = "test_key",
            .buffer_size = 8
        },
        .value = &(AugmentedBuffer) {
            .buffer_pointer = "test_val",
            .buffer_size = 8
        }
    };

    insertKeyValuePair(&pool, &store, &kv);
    assert(store.current_size == 1);

    destroyMemoryPool(&pool);
}

void testKeyValueLookup()
{
    InMemoryDataStore store;
    MemoryPoolList pool;
    initializeInMemoryDataStore(&store);
    initializeMemoryPool(&pool);

    KeyValueEntity kv = {
        .key = &(AugmentedBuffer) {
            .buffer_pointer = "test_key",
            .buffer_size = 8
        },
        .value = &(AugmentedBuffer) {
            .buffer_pointer = "test_value",
            .buffer_size = 10
        }
    };

    insertKeyValuePair(&pool, &store, &kv);

    AugmentedBuffer lookup_key = {
        .buffer_pointer = "test_key",
        .buffer_size = 8
    };
    AugmentedBuffer wrong_lookup_key = {
        .buffer_pointer = "bla",
        .buffer_size = 3
    };

    AugmentedBuffer value_buff;
    AugmentedBuffer no_value_buff;

    findValue(&pool, &store, &lookup_key, &value_buff);
    findValue(&pool, &store, &wrong_lookup_key, &no_value_buff);

    assert(value_buff.buffer_size == 10);
    assert(memcmp(value_buff.buffer_pointer, "test_value", value_buff.buffer_size) == 0);
    assert(no_value_buff.buffer_size == -1);
    assert(no_value_buff.buffer_pointer == NULL);

    destroyMemoryPool(&pool);
}

void insertDuplicateKeyInDataStore()
{
    InMemoryDataStore store;
    MemoryPoolList pool;
    initializeInMemoryDataStore(&store);
    initializeMemoryPool(&pool);

    KeyValueEntity kv = {
        .key = &(AugmentedBuffer) {
            .buffer_pointer = "test_key",
            .buffer_size = 8
        },
        .value = &(AugmentedBuffer) {
            .buffer_pointer = "test_value",
            .buffer_size = 10
        }
    };
    KeyValueEntity kv_2 = {
        .key = &(AugmentedBuffer) {
            .buffer_pointer = "test_key",
            .buffer_size = 8
        },
        .value = &(AugmentedBuffer) {
            .buffer_pointer = "blabla",
            .buffer_size = 6
        }
    };

    insertKeyValuePair(&pool, &store, &kv);
    insertKeyValuePair(&pool, &store, &kv_2);

    AugmentedBuffer lookup_key = {
        .buffer_pointer = "test_key",
        .buffer_size = 8
    };
    AugmentedBuffer return_value;

    findValue(&pool, &store, &lookup_key, &return_value);

    assert(store.current_size == 1);
    assert(memcmp(return_value.buffer_pointer, "test_value", return_value.buffer_size) == 0);

    destroyMemoryPool(&pool);
}

int main()
{
    printf("STARTING IN MEMORY DATA STORE TEST\n");

    testInitializeInMemoryDataStore();

    testKeyValueInsertion();

    testKeyValueLookup();

    insertDuplicateKeyInDataStore();
}
