#ifndef COMMON_STATUS_H
#define COMMON_STATUS_H

#include "Error.h"

#define STATUS_SUCCESS 1
#define STATUS_FAILURE 0

typedef struct Status
{
    bool success;
    Error error;

} Status;

bool Status_IsSuccessful(const Status* result);

SizeAwareBuffer* Status_GetError(Status* result);

void Status_CleanUpResources(Status* result);

#endif
