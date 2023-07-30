#ifndef GET_VALUE_MESSAGE_RESPONSE_H
#define GET_VALUE_MESSAGE_RESPONSE_H

#include "common/SizeAwareBuffer.h"
#include "common/MemoryPool.h"

/*
 ***************************************************************************************************
 * STRUCT DECLARATIONS
 ***************************************************************************************************
*/

typedef enum GetValueMessageResponseErrorCode
{
    KEY_DOES_NOT_EXIST = 1
} GetValueMessageResponseErrorCode;

/**
 * Size     | Type      | Value Size  | Value String
 * -------------------------------------------------
 * 4 Bytes  | 4 Bytes   | 4 Bytes     | ?? Bytes
 */
typedef struct GetValueMessageResponseSuccess
{
    SizeAwareBuffer value;
} GetValueMessageResponseSuccess;

/*
 * Size    | Type     | Error Code  | Message Details Size | Message Details
 * -------------------------------------------------------------------------
 * 4 Bytes | 4 Bytes  | 4 Bytes     | 4 Bytes              | ?? Bytes
*/
typedef struct GetValueMessageResponseError
{
    GetValueMessageResponseErrorCode error_code;
    SizeAwareBuffer error_details;
} GetValueMessageResponseError;

/*
 ***************************************************************************************************
 * FUNCTIONS DECLARATIONS
 ***************************************************************************************************
*/

void GetValueMessageResponseSuccess_CalculateSize(
    const GetValueMessageResponseSuccess* response,
    uint32_t* return_size);

void GetValueMessageResponseSuccess_SerializeIntoBuffer(
    const GetValueMessageResponseSuccess* response,
    SizeAwareBuffer* return_message_bytes);

void GetValueMessageResponseSuccess_Deserialize(
    const SizeAwareBuffer* message_bytes,
    GetValueMessageResponseSuccess* return_response);

void GetValueMessageResponseSuccess_AllocateBuffer(
    const GetValueMessageResponseSuccess* response,
    SizeAwareBuffer* return_message_bytes);

void GetValueMessageResponseSuccess_DestroyBuffer(SizeAwareBuffer* message_bytes);

void GetValueMessageResponseSuccess_AllocateMessage(
    const SizeAwareBuffer* message_bytes,
    GetValueMessageResponseSuccess* return_response);

void GetValueMessageResponseSuccess_DestroyMessage(GetValueMessageResponseSuccess* response);

void GetValueMessageResponseSuccess_GetValueSizeOffset(
    const SizeAwareBuffer* message_bytes,
    uint32_t* return_value_size_offset);

void GetValueMessageResponseSuccess_GetValueSize(
    const SizeAwareBuffer* message_bytes,
    uint32_t* return_value_size);

void GetValueMessageResponseSuccess_GetValueOffset(
    const SizeAwareBuffer* message_bytes,
    uint32_t* return_value_offset);

void GetValueMessageResponseSuccess_GetValue(
    const SizeAwareBuffer* message_bytes,
    SizeAwareBuffer* return_value);

// *************************************************************************************************
// Start of error functions.
// *************************************************************************************************

void GetValueMessageResponseError_CalculateSize(
    const GetValueMessageResponseError* response,
    uint32_t* return_size);

void GetValueMessageResponseError_SerializeIntoBuffer(
    const GetValueMessageResponseError* response,
    SizeAwareBuffer* return_message_bytes);

void GetValueMessageResponseError_Deserialize(
    const SizeAwareBuffer* message_bytes,
    GetValueMessageResponseError* return_response);

void GetValueMessageResponseError_AllocateBuffer(
    const GetValueMessageResponseError* response,
    SizeAwareBuffer* return_message_bytes);

void GetValueMessageResponseError_DestroyBuffer(SizeAwareBuffer* message_bytes);

void GetValueMessageResponseError_AllocateMessage(
    const SizeAwareBuffer* message_bytes,
    GetValueMessageResponseError* return_response);

void GetValueMessageResponseError_DestroyMessage(GetValueMessageResponseError* response);

void GetValueMessageResponseError_GetErrorCodeOffset(
    const SizeAwareBuffer* message_bytes,
    uint32_t* return_code_offset);

void GetValueMessageResponseError_GetErrorCode(
    const SizeAwareBuffer* message_bytes,
    GetValueMessageResponseErrorCode* return_error_code);

void GetValueMessageResponseError_GetErrorDetailsSizeOffset(
    const SizeAwareBuffer* message_bytes,
    uint32_t* return_details_size_offset);

void GetValueMessageResponseError_GetErrorDetailsSize(
    const SizeAwareBuffer* message_bytes,
    uint32_t* return_details_size);

void GetValueMessageResponseError_GetErrorDetailsOffset(
    const SizeAwareBuffer* message_bytes,
    uint32_t* return_details_offset);

void GetValueMessageResponseError_GetErrorDetails(
    const SizeAwareBuffer* message_bytes,
    SizeAwareBuffer* return_error_details);

#endif
