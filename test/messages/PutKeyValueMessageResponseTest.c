#include "messages/PutKeyValueMessageResponse.h"
#include "messages/Common.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

static void testCalculateSize()
{
    PutKeyValueMessageResponse res = {
        .status = MESSAGE_SUCCESS
    };

    uint32_t message_size;
    PutKeyValueMessageResponse_CalculateSize(&res, &message_size);

    assert(message_size == (
        MESSAGE_SIZE_BYTE_SIZE + MESSAGE_TYPE_BYTE_SIZE + MESSAGE_SIZE_FIELD_BYTES));
}

static void testSerializeDeserialize()
{
    PutKeyValueMessageResponse res = {
        .status = MESSAGE_FAILURE
    };

    SizeAwareBuffer buffer;
    PutKeyValueMessageResponse_AllocateBuffer(&res, &buffer);
    PutKeyValueMessageResponse_SerializeIntoBuffer(&res, &buffer);

    PutKeyValueMessageResponse actual;
    PutKeyValueMessageResponse_AllocateMessage(&buffer, &res);
    PutKeyValueMessageResponse_Deserialize(&buffer, &actual);

    assert(actual.status == MESSAGE_FAILURE);

    PutKeyValueMessageResponse_DestroyBuffer(&buffer);
    PutKeyValueMessageResponse_DestroyMessage(&actual);
}

int main()
{
    printf("STARTING PUT KEY VALUE MESSAGE RESPONSE TEST\n");

    testCalculateSize();

    testSerializeDeserialize();
}
