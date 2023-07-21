#ifndef KEY_VALUE_ENTITY_H
#define KEY_VALUE_ENTITY_H

#include "common/SizeAwareBuffer.h"

typedef struct KeyValueEntity
{
    SizeAwareBuffer key;
    SizeAwareBuffer value;

} KeyValueEntity;

#endif
