// #include "datastore/InMemoryDataStore.h"
// #include "common/MemoryPool.h"
// #include <string.h>
// #include <stdio.h>
// #include <stdlib.h>

// void initializeInMemoryDataStore(InMemoryDataStore* store)
// {
//     store->current_size = 0;
//     store->pool = malloc(sizeof(MemoryPoolList));
//     initializeMemoryPool(store->pool);
// }

// void destroyInMemoryDataStore(InMemoryDataStore* store)
// {
//     destroyMemoryPool(store->pool);
//     free(store->pool);
//     store->pool = NULL;
// }

// void insertKeyValuePair(InMemoryDataStore* store, KeyValueEntity* kv_entity)
// {
//     SizeAwareBuffer* test_if_value_exists;
//     findValue(store, kv_entity->key, &test_if_value_exists);
//     if (test_if_value_exists->buffer_size == 0)
//     {
//         return;
//     }

//     uint32_t block_size = 2 * sizeof(SizeAwareBuffer) + kv_entity->key->buffer_size + kv_entity->value->buffer_size;
//     SizeAwareBuffer new_kv_buffer;
//     allocateMemoryInPool(store->pool, block_size, &new_kv_buffer);

//     SizeAwareBuffer* first_ab = (SizeAwareBuffer*)new_kv_buffer.raw_buffer;
//     first_ab->raw_buffer = new_kv_buffer.raw_buffer + (2 * sizeof(SizeAwareBuffer));
//     first_ab->buffer_size = kv_entity->key->buffer_size;

//     SizeAwareBuffer* second_ab = (SizeAwareBuffer*)((char*)new_kv_buffer.raw_buffer + sizeof(SizeAwareBuffer));
//     second_ab->raw_buffer = new_kv_buffer.raw_buffer + (2 * sizeof(SizeAwareBuffer)) + kv_entity->key->buffer_size;
//     second_ab->buffer_size = kv_entity->value->buffer_size;

//     memcpy(first_ab->raw_buffer, kv_entity->key->raw_buffer, kv_entity->key->buffer_size);
//     memcpy(second_ab->raw_buffer, kv_entity->value->raw_buffer, kv_entity->value->buffer_size);

//     store->data[store->current_size].key = first_ab;
//     store->data[store->current_size].value = second_ab;

//     store->current_size++;
// }

// void findValue(InMemoryDataStore* store, const SizeAwareBuffer* key, SizeAwareBuffer** return_value)
// {
//     for (uint32_t i = 0; i < store->current_size; i++)
//     {   
//         if (SizeAwareBuffer_AreContentsSame(key, store->data[i].key))
//         {
//             *return_value = store->data[i].value;
//             return;
//         }
//     }

//     // TODO: Fix this uncleaned memory
//     *return_value = malloc(sizeof(SizeAwareBuffer));
//     (*return_value)->raw_buffer = NULL;
//     (*return_value)->buffer_size = 0;
// }
