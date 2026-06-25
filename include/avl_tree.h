#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdbool.h>
#include "data_structure.h"

typedef struct AvlNode {
    CourseRecord data;
    int height;
    struct AvlNode *left;
    struct AvlNode *right;
} AvlNode;

typedef struct {
    AvlNode *root;
    int size;
} AvlTree;

void AvlTreeInit(AvlTree *tree);
void AvlTreeDestroy(AvlTree *tree);
bool AvlTreeInsert(AvlTree *tree, const CourseRecord *record);
bool AvlTreeDelete(AvlTree *tree, const char *student_id, const char *course_id);
CourseRecord *AvlTreeFind(AvlTree *tree, const char *student_id, const char *course_id);
void AvlTreeTraverse(const AvlTree *tree, void (*visit)(const CourseRecord *));

#endif
