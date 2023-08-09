#ifndef VOID_BUFFER_H
#define VOID_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct VoidBuffer
{
    uint32_t number_of_bytes;
    void* data;
} VoidBuffer;

void VoidBuffer_Allocate(uint32_t number_of_bytes, VoidBuffer* return_buffer);

void VoidBuffer_DeepCopy(const VoidBuffer* buffer, VoidBuffer* return_buffer);

void VoidBuffer_ShallowCopy(VoidBuffer* buffer, VoidBuffer* return_buffer);

void VoidBuffer_Destroy(VoidBuffer* buffer);

bool VoidBuffer_AreEqual(const VoidBuffer* buffer_1, const VoidBuffer* buffer_2);

void VoidBuffer_Print(const VoidBuffer* buffer);

#endif
