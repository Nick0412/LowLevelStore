#include "common/Error.h"

void printError(Error* error)
{
    printString(error->error_string);
}

void initializeError(Error* error, int32_t error_code, StringBuffer* error_string)
{
    error->error_code = error_code;
    error->error_string = error_string;
}
