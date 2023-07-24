#include "common/Error.h"

#include <stdlib.h>
#include <stdio.h>

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

SizeAwareBuffer* KeyAlreadyExistsInStoreError_Describe(void* self)
{
    KeyAlreadyExistsInStoreError* cast_error = (KeyAlreadyExistsInStoreError*)self;
    return &cast_error->return_message;
}

void KeyAlreadyExistsInStoreError_Destroy(void* self)
{
    KeyAlreadyExistsInStoreError* cast_error = (KeyAlreadyExistsInStoreError*)self;
    SizeAwareBuffer_DestroyBuffer(&cast_error->return_message);
    free(cast_error);
}

Error KeyAlreadyExistsInStoreError_Create(const SizeAwareBuffer* key)
{
    char* message_format = "The key already exists inside the data store. Key: %.*s";
    uint32_t num_message_bytes = snprintf(
        NULL, 0, message_format, key->buffer_size, key->raw_buffer);

    KeyAlreadyExistsInStoreError* specific_error = malloc(sizeof(KeyAlreadyExistsInStoreError));
    SizeAwareBuffer_AllocateBuffer(num_message_bytes, &specific_error->return_message);
    snprintf((char*)specific_error->return_message.raw_buffer,
        num_message_bytes+1,
        message_format,
        key->buffer_size,
        key->raw_buffer);

    Error return_error = {
        .describeError = &KeyAlreadyExistsInStoreError_Describe,
        .destroyError = &KeyAlreadyExistsInStoreError_Destroy,
        .self_error_data = specific_error
    };

    return return_error;
}

SizeAwareBuffer* CanNotFindKeyInStoreError_Describe(void* self)
{
    CanNotFindKeyInStoreError* err = (CanNotFindKeyInStoreError*)self;
    return &err->return_message;
}

void CanNotFindKeyInStoreError_Destroy(void* self)
{
    CanNotFindKeyInStoreError* cast_error = (CanNotFindKeyInStoreError*)self;
    SizeAwareBuffer_DestroyBuffer(&cast_error->return_message);
    free(cast_error);
}

Error CanNotFindKeyInStoreError_Create(const SizeAwareBuffer* key)
{
    char* message_format = "Can not find key in data store. Key: %.*s";
    uint32_t num_message_bytes = snprintf(
        NULL, 0, message_format, key->buffer_size, key->raw_buffer);
    
    CanNotFindKeyInStoreError* specific_error = malloc(sizeof(CanNotFindKeyInStoreError));
    SizeAwareBuffer_AllocateBuffer(num_message_bytes, &specific_error->return_message);
    snprintf((char*)specific_error->return_message.raw_buffer,
        num_message_bytes+1,
        message_format,
        key->buffer_size,
        key->raw_buffer);

    Error return_error = {
        .describeError = &CanNotFindKeyInStoreError_Describe,
        .destroyError = &CanNotFindKeyInStoreError_Destroy,
        .self_error_data = specific_error
    };

    return return_error;
}
