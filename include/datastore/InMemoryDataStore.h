#ifndef IN_MEMORY_DATA_STORE_H
#define IN_MEMORY_DATA_STORE_H

#include "KeyValueEntity.h"
#include "common/MemoryPool.h"

#define MAX_NUMBER_OF_KVS 100

/**
 * @brief Naive construct of an in memory data store comprise of an append only
 * array of key value structs.
 * 
 */
typedef struct InMemoryDataStore
{
    uint32_t current_size;
    KeyValueEntity data[MAX_NUMBER_OF_KVS];
    MemoryPoolList* pool;

} InMemoryDataStore;

void initializeInMemoryDataStore(InMemoryDataStore* store);

void destroyInMemoryDataStore(InMemoryDataStore* store);

/**
 * @brief 
 * 
 * KeyValueEntity
 *   SizeAwareBuffer* (key)
 *     void* (buffer pointer)
 *     uint32_t (size)
 *   SizeAwareBuffer* (value)
 *     void* (buffer pointer)
 *     uint32_t (size)
 * 
 * Every key value we insert into the data store we need to allocate memory as follow
 * 2 SizeAwareBuffers for the key and value augmented buffers
 * 2 void* to hold the key and value strings
 * 
 * @param store 
 * @param kv_entity 
 */
void insertKeyValuePair(InMemoryDataStore* store, KeyValueEntity* kv_entity);

/**
 * @brief 
 * 
 * `return_value` is a pointer to the memory inside the data store, as opposed to a copy of that data.
 * 
 * @param store 
 * @param key 
 * @param return_value 
 */
void findValue(InMemoryDataStore* store, const SizeAwareBuffer* key, SizeAwareBuffer** return_value);

#endif
