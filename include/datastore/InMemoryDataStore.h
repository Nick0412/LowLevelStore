#ifndef IN_MEMORY_DATA_STORE_H
#define IN_MEMORY_DATA_STORE_H

#define MAX_NUMBER_OF_KVS 100

#include "KeyValueEntity.h"
#include "common/MemoryPool.h"

/**
 * @brief Naive construct of an in memory data store comprise of an append only
 * array of key value structs.
 * 
 */
typedef struct InMemoryDataStore
{
    uint32_t current_size;
    KeyValueEntity data[MAX_NUMBER_OF_KVS];

} InMemoryDataStore;

void initializeInMemoryDataStore(InMemoryDataStore* store);

/**
 * @brief 
 * 
 * KeyValueEntity
 *   AugmentedBuffer* (key)
 *     void* (buffer pointer)
 *     uint32_t (size)
 *   AugmentedBuffer* (value)
 *     void* (buffer pointer)
 *     uint32_t (size)
 * 
 * Every key value we insert into the data store we need to allocate memory as follow
 * 2 AugmentedBuffers for the key and value augmented buffers
 * 2 void* to hold the key and value strings
 * 
 * @param store 
 * @param kv_entity 
 */
void insertKeyValuePair(MemoryPoolList* pool, InMemoryDataStore* store, KeyValueEntity* kv_entity);

void findValue(MemoryPoolList* pool, const InMemoryDataStore* store, const AugmentedBuffer* key, AugmentedBuffer* return_value);

#endif
