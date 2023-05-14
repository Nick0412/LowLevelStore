#include "common/Error.h"

void initializeError(Error* error, int32_t error_code, AugmentedBuffer* error_string)
{
    error->error_code = error_code;
    error->error_string = error_string;
}
