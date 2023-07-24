#include "datastore/InMemoryDataStore.h"
#include <stdlib.h>
#include <string.h>


void InMemoryDataStore_Initialize(InMemoryDataStore* store)
{
    store->current_size = 0;
    MemoryPool_Initialize(&store->pool);
}

void InMemoryDataStore_Destroy(InMemoryDataStore* store)
{
    MemoryPool_DestroyPool(&store->pool);
    store->current_size = 0;
}

Status InMemoryDataStore_InsertKeyValuePair(InMemoryDataStore* store, const KeyValueEntity* kv)
{
    Status status;
    SizeAwareBuffer* temp_ptr;
    Status is_key_in_store = InMemoryDataStore_FindValue(store, &kv->key, &temp_ptr);

    if (Status_IsSuccessful(&is_key_in_store))
    {
        status.success = STATUS_FAILURE;
        status.error = KeyAlreadyExistsInStoreError_Create(&kv->key);
        return status;
    }

    Status_CleanUpResources(&is_key_in_store);

    // Allocate memory for the key and value strings.
    uint32_t num_bytes_for_kv = kv->key.buffer_size + kv->value.buffer_size;
    SizeAwareBuffer* buff_for_kv;
    MemoryPool_AllocateMemorySize(&store->pool, num_bytes_for_kv, &buff_for_kv);

    // Put the key and value strings.
    SizeAwareBuffer_PlaceStringInBuffer(&kv->key, buff_for_kv, 0);
    SizeAwareBuffer_PlaceStringInBuffer(&kv->value, buff_for_kv, kv->key.buffer_size);

    // Set the key value's pointers to the new allocated memory correctly.
    store->data[store->current_size] = (KeyValueEntity){
        .key = { .buffer_size = kv->key.buffer_size, .raw_buffer = buff_for_kv->raw_buffer },
        .value = {
            .buffer_size = kv->value.buffer_size,
            .raw_buffer = buff_for_kv->raw_buffer + kv->key.buffer_size
        }
    };

    store->current_size++;

    status.success = STATUS_SUCCESS;
    return status;
}

Status InMemoryDataStore_FindValue(
    InMemoryDataStore* store,
    const SizeAwareBuffer* key,
    SizeAwareBuffer** return_value)
{
    Status status;

    for (uint32_t i = 0; i < store->current_size; i++)
    {
        KeyValueEntity current_kv = store->data[i];
        if (SizeAwareBuffer_AreContentsSame(&current_kv.key, key))
        {
            *return_value = &current_kv.value;
            status.success = STATUS_SUCCESS;
            return status;
        }
    }

    status.success = STATUS_FAILURE;
    status.error = CanNotFindKeyInStoreError_Create(key);
    return status;
}
