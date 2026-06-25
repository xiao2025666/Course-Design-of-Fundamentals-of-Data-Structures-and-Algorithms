#include "hash_table.h"

#include <stdlib.h>
#include <string.h>

static int IndexFor(const HashTable *table, const char *student_id, const char *course_id) {
    unsigned int hash = hash_code(student_id) ^ (hash_code(course_id) << 1);
    return (int)(hash % (unsigned int)table->capacity);
}

static HashNode *CreateNode(const CourseRecord *record) {
    HashNode *node = (HashNode *)malloc(sizeof(HashNode));
    if (node == NULL) {
        return NULL;
    }
    node->data = *record;
    node->next = NULL;
    return node;
}

void HashTableInit(HashTable *table) {
    if (table == NULL) {
        return;
    }
    table->capacity = 101;
    table->size = 0;
    table->buckets = (HashNode **)calloc(table->capacity, sizeof(HashNode *));
}

void HashTableDestroy(HashTable *table) {
    if (table == NULL) {
        return;
    }
    for (int i = 0; i < table->capacity; ++i) {
        HashNode *node = table->buckets[i];
        while (node != NULL) {
            HashNode *next = node->next;
            free(node);
            node = next;
        }
    }
    free(table->buckets);
    table->buckets = NULL;
    table->capacity = 0;
    table->size = 0;
}

bool HashTableInsert(HashTable *table, const CourseRecord *record) {
    if (table == NULL || record == NULL) {
        return false;
    }

    int index = IndexFor(table, record->student_id, record->course_id);
    HashNode *node = CreateNode(record);
    if (node == NULL) {
        return false;
    }

    node->next = table->buckets[index];
    table->buckets[index] = node;
    table->size++;
    return true;
}

bool HashTableDelete(HashTable *table, const char *student_id, const char *course_id) {
    (void)table; (void)student_id; (void)course_id;
    return false;
}

CourseRecord *HashTableFind(HashTable *table, const char *student_id, const char *course_id) {
    if (table == NULL || student_id == NULL || course_id == NULL) {
        return NULL;
    }

    int index = IndexFor(table, student_id, course_id);
    HashNode *node = table->buckets[index];
    while (node != NULL) {
        if (strcmp(node->data.student_id, student_id) == 0 && strcmp(node->data.course_id, course_id) == 0) {
            return &node->data;
        }
        node = node->next;
    }
    return NULL;
}

void HashTableTraverse(const HashTable *table, void (*visit)(const CourseRecord *)) {
    if (table == NULL || visit == NULL) {
        return;
    }
    for (int i = 0; i < table->capacity; ++i) {
        HashNode *node = table->buckets[i];
        while (node != NULL) {
            visit(&node->data);
            node = node->next;
        }
    }
}
