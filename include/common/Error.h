#ifndef ERROR_H
#define ERROR_H

#include <stdint.h>
#include "StringBuffer.h"

typedef struct Error
{
    int32_t error_code;
    StringBuffer* error_string;
} Error;

void printError(Error* error);

void initializeError(Error* error, int32_t error_code, StringBuffer* error_string);

#endif
