#ifndef COMMON_RESULT_H
#define COMMON_RESULT_H

#include "SizeAwareBuffer.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum ErrorType
{
    ERROR_TYPE_EMPTY = 0,
    ERROR_TYPE_LINKED_LIST = 1
} ErrorType;

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

