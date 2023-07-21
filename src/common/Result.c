#include "common/Result.h"

bool Result_IsSuccessful(const Result* result)
{
    return result->success;
}

SizeAwareBuffer* Result_GetError(Result* result)
{
    return Error_DescribeError(&result->error);
}

void Result_CleanUpResources(Result* result)
{
    Error_DestroyError(&result->error);
}
