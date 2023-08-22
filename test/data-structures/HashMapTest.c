#include "data-structures/HashMap.h"
#include <stdio.h>
#include <assert.h>

void insertionHelper(HashMap* map, uint32_t number)
{
    char key[64] = { 'k', 'e', 'y' };
    char value[64] = { 'v', 'a', 'l', 'u', 'e' };
    char* key_offset = (char*)key + 3;
    char* value_offset = (char*)value + 5;
    for (uint32_t i = 0; i < number; i++)
    {
        int number_bytes = sprintf(key_offset, "%d", i);
        sprintf(value_offset, "%d", i);

        VoidBuffer key_buff = { .number_of_bytes = 3 + number_bytes, .data = key };
        VoidBuffer value_buff = { .number_of_bytes = 5 + number_bytes, .data = value };

        HashMap_InsertPair(map, &key_buff, &value_buff);
    }
}

void testInitialize()
{
    HashMap map;
    HashMap_InitializeWithCapacity(&map, 2);

    assert(map.capacity == 2);
    assert(map.size == 0);
    assert(map.buckets != NULL);
    assert(map.buckets[0].size == 0);
    assert(map.buckets[1].size == 0);

    HashMap_Destroy(&map);

    assert(map.buckets == NULL);
    assert(map.capacity == 0);
    assert(map.size == 0);
}

void testInsertion()
{
    HashMap map;
    HashMap_InitializeWithCapacity(&map, 3);

    char* key1 = "dog";
    char* val1 = "12345";
    char* key2 = "hello";
    char* val2 = "abcde";

    VoidBuffer k1 = { .data = key1, .number_of_bytes = strlen(key1) };
    VoidBuffer v1 = { .data = val1, .number_of_bytes = strlen(val1) };
    VoidBuffer k2 = { .data = key2, .number_of_bytes = strlen(key2) };
    VoidBuffer v2 = { .data = val2, .number_of_bytes = strlen(val2) };
    Result res1 = HashMap_InsertPair(&map, &k1, &v1);
    Result res2 = HashMap_InsertPair(&map, &k2, &v2);

    assert(map.size == 2);
    assert(map.capacity == 3);
    assert(Result_IsSuccessful(&res1));
    assert(Result_IsSuccessful(&res2));

    VoidBuffer v1_in_map;
    VoidBuffer v2_in_map;
    Result res3 = HashMap_FindValue(&map, &k1, &v1_in_map);
    Result res4 = HashMap_FindValue(&map, &k2, &v2_in_map);

    assert(Result_IsSuccessful(&res3));
    assert(Result_IsSuccessful(&res4));
    assert(VoidBuffer_AreEqual(&v1, &v1_in_map));
    assert(VoidBuffer_AreEqual(&v2, &v2_in_map));

    HashMap_Destroy(&map);
}

void testResizing()
{
    HashMap map;
    HashMap_InitializeWithCapacity(&map, 5);
    insertionHelper(&map, 6);

    assert(map.capacity == 10);
    assert(map.size == 6);

    HashMap_Destroy(&map);
    assert(map.capacity == 0);
    assert(map.size == 0);
}

void testGetValueAfterResize()
{
    HashMap map;
    HashMap_InitializeWithCapacity(&map, 5);
    insertionHelper(&map, 6);

    VoidBuffer key1 = { .data = "key1", .number_of_bytes = 4 };
    VoidBuffer key2 = { .data = "key2", .number_of_bytes = 4 };
    VoidBuffer key5 = { .data = "key5", .number_of_bytes = 4 };
    VoidBuffer value1;
    VoidBuffer value2;
    VoidBuffer value5;

    HashMap_FindValue(&map, &key1, &value1);
    HashMap_FindValue(&map, &key2, &value2);
    HashMap_FindValue(&map, &key5, &value5);

    VoidBuffer expected_value1 = { .data = "value1", .number_of_bytes = 6 };
    VoidBuffer expected_value2 = { .data = "value2", .number_of_bytes = 6 };
    VoidBuffer expected_value5 = { .data = "value5", .number_of_bytes = 6 };

    assert(VoidBuffer_AreEqual(&expected_value1, &value1));
    assert(VoidBuffer_AreEqual(&expected_value2, &value2));
    assert(VoidBuffer_AreEqual(&expected_value5, &value5));

    HashMap_Destroy(&map);
    VoidBuffer_Destroy(&value1);
    VoidBuffer_Destroy(&value2);
    VoidBuffer_Destroy(&value5);
}

int main()
{
    printf("STARTING HASH MAP TEST\n");

    testInitialize();

    testInsertion();

    testResizing();

    testGetValueAfterResize();
}
