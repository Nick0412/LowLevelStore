#include "messages/PutKeyValueMessageRequest.h"
#include "messages/Constants.h"
#include <stdlib.h>
#include <stdio.h>

void PutKeyValueMessageRequest_CalculateSize(const PutKeyValueMessageRequest* request, uint32_t* return_request_size)
{
    uint32_t size = 0;
    
    size += MESSAGE_SIZE_BYTE_SIZE;
    size += MESSAGE_TYPE_BYTE_SIZE;
}