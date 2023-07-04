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
        .key = &(SizeAwareBuffer) {
            .raw_buffer = (uint8_t*)"test_key",
            .buffer_size = 8
        },
        .value = &(SizeAwareBuffer) {
            .raw_buffer = (uint8_t*)"test_value",
            .buffer_size = 10
        }
    };

    insertKeyValuePair(&data_store, &input_kv);

    SizeAwareBuffer lookup_key = {
        .raw_buffer = (uint8_t*)"test_key",
        .buffer_size = 8
    };

    SizeAwareBuffer* return_value;
    findValue(&data_store, &lookup_key, &return_value);

    assert(memcmp(return_value->raw_buffer, "test_value", 10) == 0);
    assert(return_value->buffer_size == 10);

    destroyInMemoryDataStore(&data_store);
}

int main()
{
    printf("STARTING IN MEMORY DATA STORE TEST\n");

    testInitializeInMemoryDataStore();

    testKeyValueInsertion();
}
