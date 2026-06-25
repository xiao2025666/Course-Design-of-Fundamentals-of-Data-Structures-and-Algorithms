#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdbool.h>
#include "data_structure.h"

typedef struct HashNode {
    CourseRecord data;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode **buckets;
    int capacity;
    int size;
} HashTable;

void HashTableInit(HashTable *table);
void HashTableDestroy(HashTable *table);
bool HashTableInsert(HashTable *table, const CourseRecord *record);
bool HashTableDelete(HashTable *table, const char *student_id, const char *course_id);
CourseRecord *HashTableFind(HashTable *table, const char *student_id, const char *course_id);
void HashTableTraverse(const HashTable *table, void (*visit)(const CourseRecord *));

#endif
