#include "common/Error.h"
#include "common/SizeAwareBuffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct SampleError 
{
    SizeAwareBuffer* message;
} SampleError;

void SampleError_DestroyError(void* self)
{
    SampleError* err = (SampleError*)self;
    SizeAwareBuffer_DestroyBuffer(err->message);
    free(err->message);
    free(err);
}

SizeAwareBuffer* SampleError_DescribeError(void* self)
{
    SampleError* err = (SampleError*)self;
    return err->message;
}

Error CreateSampleError()
{
    SampleError* sample_error = malloc(sizeof(SampleError));
    sample_error->message = malloc(sizeof(SizeAwareBuffer));

    char msg[] = "Some sample error";
    SizeAwareBuffer_AllocateBuffer(sizeof(msg)-1, sample_error->message);
    memcpy(sample_error->message->raw_buffer, msg, sizeof(msg)-1);

    Error err = {
        .destroyError = SampleError_DestroyError,
        .describeError = SampleError_DescribeError,
        .self_error_data = sample_error
    };

    return err;
}

void verifySampleError()
{
    printf("  - verifySampleError\n");

    Error err = CreateSampleError();
    SizeAwareBuffer* msg = Error_DescribeError(&err);

    assert(memcmp(msg->raw_buffer, "Some sample error", msg->buffer_size) == 0);

    Error_DestroyError(&err);
}

void verifyKeyAlreadyExistsError()
{
    printf("  - verifyKeyAlreadyExistsError\n");

    // Setup
    char* test_message = "The key already exists inside the data store. Key: hello1";
    SizeAwareBuffer key = {
        .raw_buffer = (uint8_t*)"hello1",
        .buffer_size = 6
    };
    SizeAwareBuffer expected = {
        .buffer_size = strlen(test_message),
        .raw_buffer = (uint8_t*)test_message
    };

    // Act
    Error err = KeyAlreadyExistsInStoreError_Create(&key);

    // Verify
    SizeAwareBuffer* actual = Error_DescribeError(&err);
    assert(SizeAwareBuffer_AreContentsSame(&expected, actual));

    // Cleanup
    Error_DestroyError(&err);
}

void verifyCanNotFindKeyError()
{
    printf("  - verifyCanNotFindKeyError\n");

    // Setup
    char* test_message = "Can not find key in data store. Key: test123";
    SizeAwareBuffer key = {
        .raw_buffer = (uint8_t*)"test123",
        .buffer_size = 7
    };
    SizeAwareBuffer expected = {
        .buffer_size = strlen(test_message),
        .raw_buffer = (uint8_t*)test_message
    };

    // Act
    Error err = CanNotFindKeyInStoreError_Create(&key);

    // Verify
    SizeAwareBuffer* actual = Error_DescribeError(&err);
    assert(SizeAwareBuffer_AreContentsSame(&expected, actual));

    // Cleanup
    Error_DestroyError(&err);
}

int main()
{
    printf("STARTING ERROR TEST\n");

    verifySampleError();

    verifyKeyAlreadyExistsError();

    verifyCanNotFindKeyError();
}
