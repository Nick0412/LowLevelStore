// #ifndef IN_MEMORY_DATA_STORE_H
// #define IN_MEMORY_DATA_STORE_H

// #include "KeyValueEntity.h"
// #include "common/MemoryPool.h"

// #define MAX_NUMBER_OF_KVS 100

// typedef struct InMemoryDataStore
// {
//     uint32_t current_size;
//     KeyValueEntity data[MAX_NUMBER_OF_KVS];
//     MemoryPool pool;

// } InMemoryDataStore;

// typedef struct KeyAlreadyExistsError
// {
//     SizeAwareBuffer message;

// } KeyAlreadyExistsError;



// void InMemoryDataStore_Initialize(InMemoryDataStore* store);

// void InMemoryDataStore_Destroy(InMemoryDataStore* store);

// void InMemoryDataStore_InsertKeyValuePair(InMemoryDataStore* store, const KeyValueEntity* kv);

// void InMemoryDataStore_FindValue(
//     InMemoryDataStore* store,
//     const SizeAwareBuffer* key,
//     const SizeAwareBuffer** return_value);

// #endif
