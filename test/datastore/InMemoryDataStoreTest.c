#include "datastore/InMemoryDataStore.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void testInMemoryDataStoreInitialization()
{
    InMemoryDataStore store;

    initializeInMemoryDataStore(&store, 100);

    assert(store.size == 100);
    assert(store.pairs != NULL);
    assert(store.offset == 0);

    destroyInMemoryDataStore(&store);

    assert(store.size == 0);
    assert(store.pairs == NULL);
    assert(store.offset == 0);
}

void testInMemoryDataStoreInsertAndGetSuccess()
{
    InMemoryDataStore store;
    // Nested compound literal for easier testing.
    KeyValueEntity entity = {
        .key = &(StringBuffer) {
            .string_pointer = "test",
            .string_size = 4
        },
        .value = &(StringBuffer) {
            .string_pointer = "value",
            .string_size = 5
        }
    };
    StringBuffer lookup_key = {
        .string_pointer = "test",
        .string_size = 4
    };

    initializeInMemoryDataStore(&store, 100);
    putKeyValuePair(&store, &entity);

    StringBuffer* result = getValue(&store, &lookup_key);

    assert(store.offset == 1);
    assert(result != NULL);
    assert(result->string_size == 5);
    assert(strncmp(result->string_pointer, "value", 5) == 0);

    destroyInMemoryDataStore(&store);
}

int main()
{
    printf("STARTING IN MEMORY DATA STORE TEST\n");

    testInMemoryDataStoreInitialization();

    testInMemoryDataStoreInsertAndGetSuccess();
}
