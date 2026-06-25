#include "avl_tree.h"

#include <stdlib.h>
#include <string.h>

static int MaxInt(int a, int b) { return a > b ? a : b; }

static int Height(const AvlNode *node) { return node == NULL ? 0 : node->height; }

static int BalanceFactor(const AvlNode *node) {
    if (node == NULL) {
        return 0;
    }
    return Height(node->left) - Height(node->right);
}

static void UpdateHeight(AvlNode *node) {
    if (node != NULL) {
        node->height = MaxInt(Height(node->left), Height(node->right)) + 1;
    }
}

static AvlNode *RotateLeft(AvlNode *node) {
    AvlNode *right = node->right;
    node->right = right->left;
    right->left = node;
    UpdateHeight(node);
    UpdateHeight(right);
    return right;
}

static AvlNode *RotateRight(AvlNode *node) {
    AvlNode *left = node->left;
    node->left = left->right;
    left->right = node;
    UpdateHeight(node);
    UpdateHeight(left);
    return left;
}

static AvlNode *RotateLeftRight(AvlNode *node) {
    node->left = RotateLeft(node->left);
    return RotateRight(node);
}

static AvlNode *RotateRightLeft(AvlNode *node) {
    node->right = RotateRight(node->right);
    return RotateLeft(node);
}

static AvlNode *CreateNode(const CourseRecord *record) {
    AvlNode *node = (AvlNode *)malloc(sizeof(AvlNode));
    if (node == NULL) {
        return NULL;
    }
    node->data = *record;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static AvlNode *InsertNode(AvlNode *node, const CourseRecord *record) {
    if (node == NULL) {
        return CreateNode(record);
    }

    if (strcmp(record->student_id, node->data.student_id) < 0 ||
        (strcmp(record->student_id, node->data.student_id) == 0 && strcmp(record->course_id, node->data.course_id) < 0)) {
        node->left = InsertNode(node->left, record);
    } else {
        node->right = InsertNode(node->right, record);
    }

    UpdateHeight(node);
    int balance = BalanceFactor(node);

    if (balance > 1) {
        if (BalanceFactor(node->left) >= 0) {
            return RotateRight(node);
        }
        return RotateLeftRight(node);
    }
    if (balance < -1) {
        if (BalanceFactor(node->right) <= 0) {
            return RotateLeft(node);
        }
        return RotateRightLeft(node);
    }

    return node;
}

static AvlNode *FindNode(AvlNode *node, const char *student_id, const char *course_id) {
    if (node == NULL) {
        return NULL;
    }

    int cmp_student = strcmp(student_id, node->data.student_id);
    int cmp_course = strcmp(course_id, node->data.course_id);

    if (cmp_student == 0 && cmp_course == 0) {
        return node;
    }

    if (cmp_student < 0 || (cmp_student == 0 && cmp_course < 0)) {
        return FindNode(node->left, student_id, course_id);
    }
    return FindNode(node->right, student_id, course_id);
}

static void DestroyNode(AvlNode *node) {
    if (node == NULL) {
        return;
    }
    DestroyNode(node->left);
    DestroyNode(node->right);
    free(node);
}

static void TraverseNode(const AvlNode *node, void (*visit)(const CourseRecord *)) {
    if (node == NULL || visit == NULL) {
        return;
    }
    TraverseNode(node->left, visit);
    visit(&node->data);
    TraverseNode(node->right, visit);
}

void AvlTreeInit(AvlTree *tree) {
    if (tree == NULL) {
        return;
    }
    tree->root = NULL;
    tree->size = 0;
}

void AvlTreeDestroy(AvlTree *tree) {
    if (tree == NULL) {
        return;
    }
    DestroyNode(tree->root);
    tree->root = NULL;
    tree->size = 0;
}

bool AvlTreeInsert(AvlTree *tree, const CourseRecord *record) {
    if (tree == NULL || record == NULL) {
        return false;
    }
    tree->root = InsertNode(tree->root, record);
    tree->size++;
    return true;
}

bool AvlTreeDelete(AvlTree *tree, const char *student_id, const char *course_id) {
    (void)tree; (void)student_id; (void)course_id;
    return false;
}

CourseRecord *AvlTreeFind(AvlTree *tree, const char *student_id, const char *course_id) {
    if (tree == NULL || student_id == NULL || course_id == NULL) {
        return NULL;
    }
    AvlNode *node = FindNode(tree->root, student_id, course_id);
    return node == NULL ? NULL : &node->data;
}

void AvlTreeTraverse(const AvlTree *tree, void (*visit)(const CourseRecord *)) {
    if (tree == NULL) {
        return;
    }
    TraverseNode(tree->root, visit);
}
