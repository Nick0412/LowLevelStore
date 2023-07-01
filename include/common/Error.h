#ifndef ERROR_H
#define ERROR_H

#include "SizeAwareBuffer.h"

typedef struct Error
{
    void (*destroyError)(void*);
    SizeAwareBuffer* (*describeError)(void*);
    void* self_error_data;

} Error;

SizeAwareBuffer* Error_DescribeError(Error* error);

void Error_DestroyError(Error* error);

#endif
