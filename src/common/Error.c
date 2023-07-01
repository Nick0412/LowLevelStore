#include "common/Error.h"

#include <stdlib.h>

SizeAwareBuffer* Error_DescribeError(Error* error)
{
    return error->describeError(error->self_error_data);
}

void Error_DestroyError(Error* error)
{
    // Not every error may need a destroy function
    if (error->destroyError != NULL)
    {
        error->destroyError(error->self_error_data);
    }
}
