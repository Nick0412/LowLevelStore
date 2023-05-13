#include "datastore/KeyValueEntity.h"
#include <stdio.h>

void initializeKeyValueEntity(KeyValueEntity* entity, StringBuffer* key, StringBuffer* value)
{
    entity->key = key;
    entity->value = value;
}

void printKeyValueEntity(KeyValueEntity* entity)
{
    printf("{\n\tKEY: ");
    printString(entity->key);
    printf("\n\tVALUE: ");
    printString(entity->value);
    printf("\n}\n");
}
