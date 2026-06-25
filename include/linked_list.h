/**
 * @file linked_list.h
 * @brief 双向链表数据结构模块 - 头文件
 * @author Xiao Zexi (xiaonobadthing@qq.com)
 * @college Changsha University
 * @major Cybersecurity
 * @class Network02
 * @studentId B20250308228
 * @version 1.0.0
 * @date 2026-06-25
 *
 * @details
 * 本模块实现了双向链表的创建、销毁、插入、删除、查找、遍历等操作。
 * 所有操作的时间复杂度：
 * - 插入（尾插）：O(1)
 * - 删除（按主键）：O(n)
 * - 查找（按主键）：O(n)
 * - 遍历：O(n)
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structure.h"

/**
 * @brief 双向链表结点。
 * @details 每个结点保存一条学生记录，并保存前驱和后继指针。
 */
typedef struct StudentNode {
    StudentRecord data;
    struct StudentNode *prev;
    struct StudentNode *next;
} StudentNode;

/**
 * @brief 双向链表容器。
 * @details 维护链表头尾指针以及当前元素数量。
 */
typedef struct {
    StudentNode *head;
    StudentNode *tail;
    int size;
} StudentList;

/**
 * @brief 初始化链表。
 * @param list 待初始化的链表对象
 */
void StudentListInit(StudentList *list);

/**
 * @brief 销毁链表，释放所有结点内存。
 * @param list 待销毁的链表对象
 */
void StudentListDestroy(StudentList *list);

/**
 * @brief 在链表尾部追加一个学生记录。
 * @param list 目标链表
 * @param record 待插入的学生记录
 * @return 插入成功返回 true，否则返回 false
 */
bool StudentListAppend(StudentList *list, const StudentRecord *record);

/**
 * @brief 按学号查找学生记录。
 * @param list 目标链表
 * @param student_id 要查找的学号
 * @return 找到则返回对应记录指针，否则返回 NULL
 */
StudentRecord *StudentListFindById(StudentList *list, const char *student_id);

/**
 * @brief 按学号删除学生记录。
 * @param list 目标链表
 * @param student_id 要删除的学号
 * @return 删除成功返回 true，否则返回 false
 */
bool StudentListDeleteById(StudentList *list, const char *student_id);

/**
 * @brief 遍历并打印链表中的所有学生记录。
 * @param list 目标链表
 */
void StudentListPrint(const StudentList *list);

#endif // LINKED_LIST_H