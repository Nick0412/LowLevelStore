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
    InMemoryDataStore data_store;
    initializeInMemoryDataStore(&data_store);

    KeyValueEntity input_kv = {
        .key = &(AugmentedBuffer) {
            .buffer_pointer = "test_key",
            .buffer_size = 8
        },
        .value = &(AugmentedBuffer) {
            .buffer_pointer = "test_value",
            .buffer_size = 10
        }
    };
    insertKeyValuePair(&data_store, &input_kv);

    printf("------------------------------------------------------------------------------------");
    assert(data_store.current_size == 1);
    printf("------------------------------------------------------------------------------------");

    AugmentedBuffer lookup_key = {
        .buffer_pointer = "test_key",
        .buffer_size = 8
    };

    AugmentedBuffer* return_value;
    printf("Before findValue\n");
    findValue(&data_store, &lookup_key, &return_value);

    assert(memcmp(return_value->buffer_pointer, "test_value", 10) == 0);
    assert(return_value->buffer_size == 10);

    printf("Before destroyInMemoryDataStore\n");
    destroyInMemoryDataStore(&data_store);
}

int main()
{
    printf("STARTING IN MEMORY DATA STORE TEST\n");

    testInitializeInMemoryDataStore();

    testKeyValueInsertion();
}
