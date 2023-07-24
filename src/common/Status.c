#include "common/Status.h"

bool Status_IsSuccessful(const Status* result)
{
    return result->success;
}

SizeAwareBuffer* Status_GetError(Status* result)
{
    return Error_DescribeError(&result->error);
}

void Status_CleanUpResources(Status* result)
{
    Error_DestroyError(&result->error);
}
