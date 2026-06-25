#include "storage_adapter.h"
#include "linked_list.h"
#include "avl_tree.h"
#include "hash_table.h"

static bool ListInsert(void *storage, const CourseRecord *record) {
    return StudentListAppend((StudentList *)storage, (const StudentRecord *)record);
}

static bool ListRemove(void *storage, const char *student_id, const char *course_id) {
    (void)course_id;
    return StudentListDeleteById((StudentList *)storage, student_id);
}

static CourseRecord *ListFind(void *storage, const char *student_id, const char *course_id) {
    (void)course_id;
    return (CourseRecord *)StudentListFindById((StudentList *)storage, student_id);
}

static void ListTraverse(void *storage, void (*visit)(const CourseRecord *)) {
    (void)storage;
    (void)visit;
}

static bool AvlInsert(void *storage, const CourseRecord *record) {
    return AvlTreeInsert((AvlTree *)storage, record);
}

static bool AvlRemove(void *storage, const char *student_id, const char *course_id) {
    return AvlTreeDelete((AvlTree *)storage, student_id, course_id);
}

static CourseRecord *AvlFind(void *storage, const char *student_id, const char *course_id) {
    return AvlTreeFind((AvlTree *)storage, student_id, course_id);
}

static void AvlTraverse(void *storage, void (*visit)(const CourseRecord *)) {
    AvlTreeTraverse((const AvlTree *)storage, visit);
}

static bool HashInsert(void *storage, const CourseRecord *record) {
    return HashTableInsert((HashTable *)storage, record);
}

static bool HashRemove(void *storage, const char *student_id, const char *course_id) {
    return HashTableDelete((HashTable *)storage, student_id, course_id);
}

static CourseRecord *HashFind(void *storage, const char *student_id, const char *course_id) {
    return HashTableFind((HashTable *)storage, student_id, course_id);
}

static void HashTraverse(void *storage, void (*visit)(const CourseRecord *)) {
    HashTableTraverse((const HashTable *)storage, visit);
}

void InitListAdapter(StorageAdapter *adapter, StudentList *storage) {
    (void)storage;
    adapter->insert = ListInsert;
    adapter->remove = ListRemove;
    adapter->find = ListFind;
    adapter->traverse = ListTraverse;
}

void InitAvlAdapter(StorageAdapter *adapter, AvlTree *storage) {
    adapter->insert = AvlInsert;
    adapter->remove = AvlRemove;
    adapter->find = AvlFind;
    adapter->traverse = AvlTraverse;
}

void InitHashAdapter(StorageAdapter *adapter, HashTable *storage) {
    adapter->insert = HashInsert;
    adapter->remove = HashRemove;
    adapter->find = HashFind;
    adapter->traverse = HashTraverse;
}
