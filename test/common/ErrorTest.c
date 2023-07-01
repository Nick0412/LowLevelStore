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
    printf("  - verifySampleError");

    Error err = CreateSampleError();
    SizeAwareBuffer* msg = Error_DescribeError(&err);

    assert(memcmp(msg->raw_buffer, "Some sample error", msg->buffer_size) == 0);

    Error_DestroyError(&err);
}

int main()
{
    printf("STARTING ERROR TEST\n");

    verifySampleError();
}
