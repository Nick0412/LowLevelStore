#include "common/Result.h"
#include "common/SizeAwareBuffer.h"

bool Result_IsSuccessful(const Result* result)
{
    return result->is_successful;
}

void Result_Destroy(Result* result)
{
    SizeAwareBuffer_DestroyBuffer(&result->error_details);
}

Result Result_Create(
    bool is_successful,
    ErrorType error_type,
    uint32_t error_code,
    const SizeAwareBuffer* error_details)
{
    Result res = {
        .error_code = error_code,
        .error_type = error_type,
        .is_successful = is_successful
    };
    SizeAwareBuffer_AllocateBuffer(error_details->buffer_size, &res.error_details);
    SizeAwareBuffer_PlaceStringInBuffer(error_details, &res.error_details, 0);

    return res;
}
