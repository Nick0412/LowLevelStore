#include "data-structures/HashMap.h"
#include "algorithms/Hashing.h"
#include <stdlib.h>
#include <stdio.h>

void HashMap_Initialize(HashMap* hash_map)
{
    HashMap_InitializeWithCapacity(hash_map, HASH_MAP_DEFAULT_SIZE);
}

void HashMap_InitializeWithCapacity(HashMap* hash_map, uint32_t capacity)
{
    hash_map->buckets = malloc(capacity * sizeof(LinkedList));
    hash_map->capacity = capacity;
    hash_map->size = 0;

    for (uint32_t i = 0; i < hash_map->capacity; i++)
    {
        LinkedList_Initialize(&hash_map->buckets[i]);
    }
}

void HashMap_Destroy(HashMap* hash_map)
{
    for (uint32_t i = 0; i < hash_map->capacity; i++)
    {
        LinkedList_Destroy(&hash_map->buckets[i]);
    }

    free(hash_map->buckets);
    hash_map->buckets = NULL;
    hash_map->size = 0;
    hash_map->capacity = 0;
}

Result HashMap_InsertPair(HashMap* hash_map, const VoidBuffer* key, const VoidBuffer* value)
{
    if (hash_map->size == hash_map->capacity)
    {
        HashMap_Internal_Resize(hash_map);
    }

    uint32_t hashed_bucket_index = Hashing_Djb2(key) % hash_map->capacity;
    LinkedList* hashed_bucket = &hash_map->buckets[hashed_bucket_index];
    
    if (HashMap_Internal_IsItemInBucket(hashed_bucket, key))
    {
        return HashMap_Internal_ItemAlreadyExistsInMapResult(key);
    }

    VoidBuffer key_value_data;
    uint32_t total_key_value_bytes = (2 * sizeof(VoidBuffer)) +
                                     key->number_of_bytes +
                                     value->number_of_bytes;

    VoidBuffer_Allocate(total_key_value_bytes, &key_value_data);
    HashMap_Internal_EncodeKeyValue(key, value, &key_value_data);
    LinkedList_InsertShallowDataAtTail(hashed_bucket, &key_value_data);
    hash_map->size++;

    return HashMap_Internal_SuccessfulResult();
}

Result HashMap_FindValue(HashMap* hash_map, const VoidBuffer* key, VoidBuffer* return_value)
{
    uint32_t hashed_bucket_index = Hashing_Djb2(key) % hash_map->capacity;
    LinkedList hashed_bucket = hash_map->buckets[hashed_bucket_index];

    LinkedListNode* curr = hashed_bucket.head;
    while (curr != NULL)
    {   
        void* base_pointer = curr->data.data;
        VoidBuffer* extracted_key = (VoidBuffer*)base_pointer;

        if (VoidBuffer_AreEqual(key, extracted_key))
        {
            void* extracted_value_offset = (uint8_t*)base_pointer + sizeof(VoidBuffer);
            VoidBuffer* extracted_value = (VoidBuffer*)extracted_value_offset;
            VoidBuffer_DeepCopy(extracted_value, return_value);

            return HashMap_Internal_SuccessfulResult();
        }
        curr = curr->next;
    }

    return HashMap_Internal_ItemDoesNotExistInMapResult(key);
}

Result HashMap_RemovePair(HashMap* hash_map, const VoidBuffer* key)
{
    uint32_t hashed_bucket_index = Hashing_Djb2(key) % hash_map->capacity;
    LinkedList* bucket = &hash_map->buckets[hashed_bucket_index];

    LinkedListNode* curr = bucket->head;
    while (curr != NULL)
    {
        void* key_struct = curr->data.data;
        VoidBuffer* key_struct_cast = (VoidBuffer*)key_struct;
        if (VoidBuffer_AreEqual(key_struct_cast, key))
        {
            LinkedList_DeleteNode(bucket, curr);
            hash_map->size--;

            return HashMap_Internal_SuccessfulResult();
        }
        curr = curr->next;
    }

    return HashMap_Internal_ItemDoesNotExistInMapResult(key);
}

void HashMap_Internal_Resize(HashMap* hash_map)
{
    uint32_t new_capacity = hash_map->capacity * 2;
    LinkedList* new_buckets = malloc(new_capacity * sizeof(LinkedList));

    for (uint32_t i = 0; i < hash_map->capacity; i++)
    {
        LinkedListNode* current_head = hash_map->buckets[i].head;
        while (current_head != NULL)
        {   
            void* key_struct = current_head->data.data;
            void* value_struct = (uint8_t*)key_struct + sizeof(VoidBuffer);
            VoidBuffer* key_struct_cast = (VoidBuffer*)key_struct;
            VoidBuffer* value_struct_cast = (VoidBuffer*)value_struct;

            uint32_t hashed_bucket_index = Hashing_Djb2(key_struct_cast) % new_capacity;
            LinkedList* bucket = &new_buckets[hashed_bucket_index];
            VoidBuffer key_value_data;
            uint32_t total_key_value_bytes = (2 * sizeof(VoidBuffer)) +
                                             key_struct_cast->number_of_bytes +
                                             value_struct_cast->number_of_bytes;
            
            VoidBuffer_Allocate(total_key_value_bytes, &key_value_data);
            HashMap_Internal_EncodeKeyValue(key_struct_cast, value_struct_cast, &key_value_data);
            LinkedList_InsertShallowDataAtTail(bucket, &key_value_data);

            current_head = current_head->next;
        }
    }

    HashMap_Internal_CleanupBuckets(hash_map->buckets, hash_map->capacity);

    hash_map->buckets = new_buckets;
    hash_map->capacity = new_capacity;
}

bool HashMap_Internal_IsItemInBucket(const LinkedList* bucket, const VoidBuffer* key)
{
    if (bucket->size == 0)
        return false;

    LinkedListNode* curr = bucket->head;
    while (curr != NULL)
    {
        VoidBuffer current_data = curr->data;
        VoidBuffer* key_in_map = (VoidBuffer*)current_data.data;

        if (VoidBuffer_AreEqual(key_in_map, key))
        {
            return true;
        }

        curr = curr->next;
    }

    return false;
}

Result HashMap_Internal_ItemAlreadyExistsInMapResult(const VoidBuffer* item)
{
    // TODO: make this error message format into a function
    char* message_format = "The item already exists in the hash map. Key: %.*s";
    uint32_t num_message_bytes = snprintf(NULL,
        0,
        message_format,
        item->number_of_bytes,
        (char*)item->data);

    SizeAwareBuffer error_details;
    SizeAwareBuffer_AllocateBuffer(num_message_bytes, &error_details);
    snprintf((char*)error_details.raw_buffer,
        num_message_bytes+1,
        message_format,
        item->number_of_bytes,
        (char*)item->data);

    Result res = {
        .is_successful = false,
        .error_type = ERROR_TYPE_HASH_MAP,
        .error_code = HASH_MAP_ERROR_ITEM_ALREADY_EXISTS,
        .error_details = error_details
    };

    return res;
}

Result HashMap_Internal_ItemDoesNotExistInMapResult(const VoidBuffer* item)
{
    char* message_format = "The item does not exist in the map. Key: %.*s";
    uint32_t num_message_bytes = snprintf(NULL,
        0,
        message_format,
        item->number_of_bytes,
        (char*)item->data);

    SizeAwareBuffer error_details;
    SizeAwareBuffer_AllocateBuffer(num_message_bytes, &error_details);
    snprintf((char*)error_details.raw_buffer,
        num_message_bytes+1,
        message_format,
        item->number_of_bytes,
        (char*)item->data);

    Result res = {
        .is_successful = false,
        .error_type = ERROR_TYPE_HASH_MAP,
        .error_code = HASH_MAP_ERROR_ITEM_DOES_NOT_EXIST,
        .error_details = error_details
    };

    return res;
}

// TODO: Consider breaking out successful result as a function in the result file.
Result HashMap_Internal_SuccessfulResult()
{
    Result res = { .is_successful = true, .error_type = ERROR_TYPE_EMPTY };
    return res;
}

void HashMap_Internal_EncodeKeyValue(
    const VoidBuffer* key,
    const VoidBuffer* value,
    VoidBuffer* return_data)
{
    void* base_pointer = return_data->data;
    void* offset_value_struct = (uint8_t*)base_pointer + sizeof(VoidBuffer);
    void* offset_key_data = (uint8_t*)offset_value_struct + sizeof(VoidBuffer);
    void* offset_value_data = (uint8_t*)offset_key_data + key->number_of_bytes;

    VoidBuffer key_copy = { .number_of_bytes = key->number_of_bytes, .data = offset_key_data };
    VoidBuffer val_copy = { .number_of_bytes = value->number_of_bytes, .data = offset_value_data };

    memcpy(base_pointer, &key_copy, sizeof(VoidBuffer));
    memcpy(offset_value_struct, &val_copy, sizeof(VoidBuffer));
    memcpy(offset_key_data, key->data, key->number_of_bytes);
    memcpy(offset_value_data, value->data, value->number_of_bytes);
}

void HashMap_Internal_CleanupBuckets(LinkedList* buckets, uint32_t number_of_buckets)
{
    for (uint32_t i = 0; i < number_of_buckets; i++)
    {
        if (buckets[i].size != 0)
        {
            LinkedList_Destroy(&buckets[i]);
        }
    }

    free(buckets);
    buckets = NULL;
}
