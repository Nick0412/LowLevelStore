#include "datastore/InMemoryDataStore.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void testStoreInitialization()
{
    InMemoryDataStore store;
    InMemoryDataStore_Initialize(&store);

    assert(store.current_size == 0);
    
    InMemoryDataStore_Destroy(&store);
}

void findValueDoesNotExist()
{
    // Setup
    InMemoryDataStore store;
    SizeAwareBuffer key = {
        .buffer_size = 5,
        .raw_buffer = (uint8_t*)"hello"
    };
    SizeAwareBuffer* temp_return;
    InMemoryDataStore_Initialize(&store);

    // Act
    Status status = InMemoryDataStore_FindValue(&store, &key, &temp_return);

    // Verify
    char* expected_message = "Can not find key in data store. Key: hello";
    SizeAwareBuffer expected_buffer = {
        .buffer_size = strlen(expected_message),
        .raw_buffer = (uint8_t*)expected_message
    };
    assert(!Status_IsSuccessful(&status));
    assert(SizeAwareBuffer_AreContentsSame(&expected_buffer, Status_GetError(&status)));

    // Cleanup
    Status_CleanUpResources(&status);
    InMemoryDataStore_Destroy(&store);
}

void insertValueAndFind()
{
    // Setup
    InMemoryDataStore store;
    InMemoryDataStore_Initialize(&store);
    SizeAwareBuffer key = {
        .raw_buffer = (uint8_t*)"test-key",
        .buffer_size = 8
    };
    SizeAwareBuffer value = {
        .raw_buffer = (uint8_t*)"test-value",
        .buffer_size = 10
    };
    KeyValueEntity kv = {
        .key = key,
        .value = value
    };

    // Act
    InMemoryDataStore_InsertKeyValuePair(&store, &kv);

    // Verify
    SizeAwareBuffer* result_ptr;
    Status status = InMemoryDataStore_FindValue(&store, &key, &result_ptr);
    assert(Status_IsSuccessful(&status));
    assert(SizeAwareBuffer_AreContentsSame(&value, result_ptr));

    // Cleanup
    InMemoryDataStore_Destroy(&store);
}

void insertKeyTwiceError()
{
    // Setup
    InMemoryDataStore store;
    InMemoryDataStore_Initialize(&store);
    SizeAwareBuffer key = { .raw_buffer = (uint8_t*)"test-key", .buffer_size = 8 };
    SizeAwareBuffer value = { .raw_buffer = (uint8_t*)"test-value", .buffer_size = 10 };
    SizeAwareBuffer value2 = { .raw_buffer = (uint8_t*)"blabla", .buffer_size = 6 };
    KeyValueEntity kv = { .key = key, .value = value };
    KeyValueEntity kv2 = { .key = key, .value = value2 };

    char* expected_message = "The key already exists inside the data store. Key: test-key";
    SizeAwareBuffer expected = { 
        .buffer_size = strlen(expected_message),
        .raw_buffer = (uint8_t*)expected_message
    };
    
    // Act
    InMemoryDataStore_InsertKeyValuePair(&store, &kv);
    Status status = InMemoryDataStore_InsertKeyValuePair(&store, &kv2);

    // Verify
    assert(!Status_IsSuccessful(&status));
    assert(SizeAwareBuffer_AreContentsSame(&expected, Status_GetError(&status)));

    // Cleanup
    Status_CleanUpResources(&status);
    InMemoryDataStore_Destroy(&store);
}

int main()
{
    printf("STARTING IN MEMORY DATA STORE TEST\n");

    testStoreInitialization();

    findValueDoesNotExist();

    insertValueAndFind();

    insertKeyTwiceError();
}
