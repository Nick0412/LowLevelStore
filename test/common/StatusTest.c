#include "common/Status.h"
#include "common/SizeAwareBuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct SampleError
{
    SizeAwareBuffer message;
} SampleError;

void SampleError_Destroy(void* self)
{
    SampleError* err = (SampleError*)self;
    SizeAwareBuffer_DestroyBuffer(&err->message);
    free(self);
}

SizeAwareBuffer* SampleError_DescribeError(void* self)
{
    SampleError* err = (SampleError*)self;
    return &err->message;
}

Status createSampleResult()
{
    SampleError* specific_error = malloc(sizeof(SampleError));
    char message[] = "This is a test error";
    SizeAwareBuffer_AllocateBuffer(sizeof(message)-1, &specific_error->message);
    memcpy(specific_error->message.raw_buffer, message, sizeof(message)-1);

    Status res = {
        .success = 1,
        .error = {
            .describeError = SampleError_DescribeError,
            .destroyError = SampleError_Destroy,
            .self_error_data = specific_error
        }
    };

    return res;
}

void testSimpleResult()
{
    printf("  - testSimpleResult\n");

    Status res = createSampleResult();
    SizeAwareBuffer* msg = Status_GetError(&res);
    bool status = Status_IsSuccessful(&res);

    assert(memcmp(msg->raw_buffer, "This is a test error", msg->buffer_size) == 0);
    assert(status == 1);

    Status_CleanUpResources(&res);
}

int main()
{
    printf("STARTING RESULT TEST\n");

    testSimpleResult();

    return 0;
}
