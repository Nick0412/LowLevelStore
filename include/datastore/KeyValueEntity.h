#ifndef KEY_VALUE_ENTITY_H
#define KEY_VALUE_ENTITY_H

#include "common/BufferHelper.h"

typedef struct KeyValueEntity
{
    AugmentedBuffer* key;
    AugmentedBuffer* value;

} KeyValueEntity;

#endif
