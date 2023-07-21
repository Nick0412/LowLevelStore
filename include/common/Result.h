#ifndef COMMON_RESULT_H
#define COMMON_RESULT_H

#include "Error.h"

typedef struct Result
{
    bool success;
    Error error;

} Result;

bool Result_IsSuccessful(const Result* result);

SizeAwareBuffer* Result_GetError(Result* result);

void Result_CleanUpResources(Result* result);

#endif
