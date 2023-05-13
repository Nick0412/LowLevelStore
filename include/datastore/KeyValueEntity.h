#ifndef KEY_VALUE_ENTITY_H
#define KEY_VALUE_ENTITY_H

#include "common/StringBuffer.h"

typedef struct KeyValueEntity
{
    StringBuffer* key;
    StringBuffer* value;

} KeyValueEntity;

void initializeKeyValueEntity(KeyValueEntity* entity, StringBuffer* key, StringBuffer* value);

void printKeyValueEntity(KeyValueEntity* entity);

#endif
