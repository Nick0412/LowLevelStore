#ifndef GET_VALUE_MESSAGE_RESPONSE_H
#define GET_VALUE_MESSAGE_RESPONSE_H

#include "common/SizeAwareBuffer.h"
#include "common/MemoryPool.h"

/*
 ***************************************************************************************************
 * STRUCT DECLARATIONS
 ***************************************************************************************************
*/

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

#endif
