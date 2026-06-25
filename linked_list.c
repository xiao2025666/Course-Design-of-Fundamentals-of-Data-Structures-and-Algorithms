/**
 * @file linked_list.c
 * @brief 双向链表数据结构模块 - 实现文件
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
 */

#include "linked_list.h"

static StudentNode *CreateStudentNode(const StudentRecord *record) {
    StudentNode *node = (StudentNode *)malloc(sizeof(StudentNode));
    if (node == NULL) {
        return NULL;
    }

    node->data = *record;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void StudentListInit(StudentList *list) {
    if (list == NULL) {
        return;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void StudentListDestroy(StudentList *list) {
    if (list == NULL) {
        return;
    }

    StudentNode *current = list->head;
    while (current != NULL) {
        StudentNode *next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

bool StudentListAppend(StudentList *list, const StudentRecord *record) {
    if (list == NULL || record == NULL) {
        return false;
    }

    StudentNode *node = CreateStudentNode(record);
    if (node == NULL) {
        return false;
    }

    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }

    list->size++;
    return true;
}

StudentRecord *StudentListFindById(StudentList *list, const char *student_id) {
    if (list == NULL || student_id == NULL) {
        return NULL;
    }

    StudentNode *current = list->head;
    while (current != NULL) {
        if (strcmp(current->data.student_id, student_id) == 0) {
            return &current->data;
        }
        current = current->next;
    }

    return NULL;
}

bool StudentListDeleteById(StudentList *list, const char *student_id) {
    if (list == NULL || student_id == NULL) {
        return false;
    }

    StudentNode *current = list->head;
    while (current != NULL) {
        if (strcmp(current->data.student_id, student_id) == 0) {
            if (current->prev != NULL) {
                current->prev->next = current->next;
            } else {
                list->head = current->next;
            }

            if (current->next != NULL) {
                current->next->prev = current->prev;
            } else {
                list->tail = current->prev;
            }

            free(current);
            list->size--;
            return true;
        }
        current = current->next;
    }

    return false;
}

void StudentListPrint(const StudentList *list) {
    if (list == NULL || list->head == NULL) {
        printf("当前链表为空。\n");
        return;
    }

    printf("当前链表共有 %d 条记录：\n", list->size);
    StudentNode *current = list->head;
    while (current != NULL) {
        printf("- 学号:%s | 姓名:%s | 专业:%s\n",
               current->data.student_id,
               current->data.student_name,
               current->data.major);
        current = current->next;
    }
}
