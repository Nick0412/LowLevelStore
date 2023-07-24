#ifndef ERROR_H
#define ERROR_H

#include "SizeAwareBuffer.h"

/***************************************************************************************************
 * STRUCT DECLARATIONS
 ***************************************************************************************************
*/

typedef struct Error
{
    void (*destroyError)(void*);
    SizeAwareBuffer* (*describeError)(void*);
    void* self_error_data;
} Error;

typedef struct KeyAlreadyExistsInStoreError
{
    SizeAwareBuffer return_message;
} KeyAlreadyExistsInStoreError;

typedef struct CanNotFindKeyInStoreError
{
    SizeAwareBuffer return_message;
} CanNotFindKeyInStoreError;

/***************************************************************************************************
 * FUNCTIONS DECLARATIONS
 ***************************************************************************************************
*/

SizeAwareBuffer* Error_DescribeError(Error* error);
void Error_DestroyError(Error* error);

SizeAwareBuffer* KeyAlreadyExistsInStoreError_Describe(void* self);
void KeyAlreadyExistsInStoreError_Destroy(void* self);
Error KeyAlreadyExistsInStoreError_Create(const SizeAwareBuffer* key);

SizeAwareBuffer* CanNotFindKeyInStoreError_Describe(void* self);
void CanNotFindKeyInStoreError_Destroy(void* self);
Error CanNotFindKeyInStoreError_Create(const SizeAwareBuffer* key);

#endif
