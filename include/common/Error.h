#ifndef ERROR_H
#define ERROR_H

#include <stdint.h>
#include "BufferHelper.h"

typedef struct Error
{
    int32_t error_code;
    AugmentedBuffer* error_string;
} Error;

void initializeError(Error* error, int32_t error_code, AugmentedBuffer* error_string);

#endif
