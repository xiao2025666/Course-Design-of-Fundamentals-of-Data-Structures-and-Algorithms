#include "storage_adapter.h"
#include "linked_list.h"
#include "avl_tree.h"
#include "hash_table.h"

/* 链表存储结构的适配器实现 */
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

/* AVL 树存储结构的适配器实现 */
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

/* 哈希表存储结构的适配器实现 */
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

/**
 * @brief 初始化链表适配器。
 * @details 将统一接口绑定到链表实现上，便于上层统一调用。
 */
void InitListAdapter(StorageAdapter *adapter, StudentList *storage) {
    (void)storage;
    adapter->insert = ListInsert;
    adapter->remove = ListRemove;
    adapter->find = ListFind;
    adapter->traverse = ListTraverse;
}

/**
 * @brief 初始化 AVL 适配器。
 */
void InitAvlAdapter(StorageAdapter *adapter, AvlTree *storage) {
    adapter->insert = AvlInsert;
    adapter->remove = AvlRemove;
    adapter->find = AvlFind;
    adapter->traverse = AvlTraverse;
}

/**
 * @brief 初始化哈希表适配器。
 */
void InitHashAdapter(StorageAdapter *adapter, HashTable *storage) {
    adapter->insert = HashInsert;
    adapter->remove = HashRemove;
    adapter->find = HashFind;
    adapter->traverse = HashTraverse;
}
