#ifndef HASHMAP_H
#define HASHMAP_H

#define HASH_MAP_DEFAULT_SIZE 64

#include "data-structures/VoidBuffer.h"
#include "data-structures/LinkedList.h"
#include "common/Result.h"

typedef struct HashMap
{
    LinkedList* buckets;
    uint32_t capacity;
    uint32_t size;
} HashMap;

void HashMap_Initialize(HashMap* hash_map);

void HashMap_InitializeWithCapacity(HashMap* hash_map, uint32_t capacity);

void HashMap_Destroy(HashMap* hash_map);

Result HashMap_InsertPair(HashMap* hash_map, const VoidBuffer* key, const VoidBuffer* value);

Result HashMap_FindValue(HashMap* hash_map, const VoidBuffer* key, VoidBuffer* return_value);

Result HashMap_RemovePair(HashMap* hash_map, const VoidBuffer* key);

void HashMap_Internal_Resize(HashMap* hash_map);

bool HashMap_Internal_IsItemInBucket(const LinkedList* bucket, const VoidBuffer* key);

Result HashMap_Internal_ItemAlreadyExistsInMapResult(const VoidBuffer* item);

Result HashMap_Internal_SuccessfulResult();

Result HashMap_Internal_ItemDoesNotExistInMapResult(const VoidBuffer* item);

/**
 * Currently the buffer is as follows
 * | Key struct bytes | Value struct bytes | Data for key | Data for value |
*/
void HashMap_Internal_EncodeKeyValue(
    const VoidBuffer* key,
    const VoidBuffer* value,
    VoidBuffer* return_data);

void HashMap_Internal_CleanupBuckets(LinkedList* buckets, uint32_t number_of_buckets);

#endif
