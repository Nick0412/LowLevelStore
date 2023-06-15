#include "messages/PutKeyValueMessageResponse.h"
#include "messages/MessageStatus.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

static void testMessageSize()
{
    PutKeyValueMessageResponse response;
    uint32_t size;
    calculatePutKeyValueMessageResponseSize(&response, &size);

    assert(size == 12);
}

static void testSerializeDeserialize()
{
    PutKeyValueMessageResponse response = {
        .status = MESSAGE_SUCCESS
    };

    uint32_t response_size;
    calculatePutKeyValueMessageResponseSize(&response, &response_size);
    AugmentedBuffer buffer = {
        .buffer_pointer = malloc(response_size),
        .buffer_size = response_size
    };

    serializePutKeyValueMessageResponse(&response, &buffer);
    PutKeyValueMessageResponse deserialized;
    deserializePutKeyValueMessageResponse(&buffer, &deserialized);

    assert(deserialized.status == MESSAGE_SUCCESS);
}

int main()
{
    printf("STARTING PUT KEY VALUE MESSAGE RESPONSE TEST\n");

    testMessageSize();

    testSerializeDeserialize();
}
