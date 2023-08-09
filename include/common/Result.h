#ifndef COMMON_RESULT_H
#define COMMON_RESULT_H

#include "SizeAwareBuffer.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum ErrorType
{
    ERROR_TYPE_EMPTY = 1,
    ERROR_TYPE_LINKED_LIST = 2,
    ERROR_TYPE_HASH_MAP
} ErrorType;

typedef enum LinkedListError
{
    LINKED_LIST_ERROR_DATA_NOT_FOUND = 1
} LinkedListError;

typedef enum HashMapError
{
    HASH_MAP_ERROR_ITEM_ALREADY_EXISTS = 1,
    HASH_MAP_ERROR_ITEM_DOES_NOT_EXIST = 2
} HashMapError;

typedef struct Result
{
    bool is_successful;
    ErrorType error_type;
    uint32_t error_code;
    SizeAwareBuffer error_details;
} Result;

bool Result_IsSuccessful(const Result* result);

void Result_Destroy(Result* result);

Result Result_Create(
    bool is_successful,
    ErrorType error_type,
    uint32_t error_code,
    const SizeAwareBuffer* error_details);

#endif

