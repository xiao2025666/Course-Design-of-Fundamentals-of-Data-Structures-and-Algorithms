/**
 * @file avl_tree.c
 * @brief AVL 平衡二叉搜索树的实现，用于存储和检索课程记录。
 *
 * 本文件实现了 AVL 树的初始化、插入、查找、遍历和销毁等操作。
 * 树节点以 (学号 student_id, 课程号 course_id) 的复合键进行排序，
 * 比较规则：先按学号升序，学号相同时按课程号升序。
 */

#include "avl_tree.h"

#include <stdlib.h>
#include <string.h>

/**
 * @brief 返回两个整数中的较大值。
 *
 * @param a 第一个整数。
 * @param b 第二个整数。
 * @return a 和 b 中的最大值。
 */
static int MaxInt(int a, int b) { return a > b ? a : b; }

/**
 * @brief 获取指定节点的高度。
 *
 * 空节点的高度定义为 0，叶节点高度为 1。
 *
 * @param node 待查询的节点指针，可为 NULL。
 * @return 节点的高度值；若 node 为 NULL 则返回 0。
 */
static int Height(const AvlNode *node) { return node == NULL ? 0 : node->height; }

/**
 * @brief 计算指定节点的平衡因子（左子树高度 - 右子树高度）。
 *
 * @param node 待计算的节点指针，可为 NULL。
 * @return 平衡因子值；若 node 为 NULL 则返回 0。
 */
static int BalanceFactor(const AvlNode *node) {
    if (node == NULL) {
        return 0;
    }
    return Height(node->left) - Height(node->right);
}

/**
 * @brief 更新指定节点的高度，使其等于左右子树高度最大值加 1。
 *
 * @param node 待更新高度的节点指针，若为 NULL 则不做任何操作。
 */
static void UpdateHeight(AvlNode *node) {
    if (node != NULL) {
        node->height = MaxInt(Height(node->left), Height(node->right)) + 1;
    }
}

/**
 * @brief 对以 node 为根的子树执行左单旋转（LL 不平衡的对称情况）。
 *
 * @param node 待旋转的子树的根节点（平衡因子 < -1 且右子树偏右）。
 * @return 旋转后新的子树根节点指针。
 */
static AvlNode *RotateLeft(AvlNode *node) {
    AvlNode *right = node->right;        /**< node 的右子节点，将成为新根 */
    node->right = right->left;           /**< 将 right 的左子树挂到 node 的右侧 */
    right->left = node;                  /**< 将原根 node 变为 right 的左子节点 */
    UpdateHeight(node);                  /**< 更新原根的高度 */
    UpdateHeight(right);                 /**< 更新新根的高度 */
    return right;
}

/**
 * @brief 对以 node 为根的子树执行右单旋转（RR 不平衡的对称情况）。
 *
 * @param node 待旋转的子树的根节点（平衡因子 > 1 且左子树偏左）。
 * @return 旋转后新的子树根节点指针。
 */
static AvlNode *RotateRight(AvlNode *node) {
    AvlNode *left = node->left;          /**< node 的左子节点，将成为新根 */
    node->left = left->right;            /**< 将 left 的右子树挂到 node 的左侧 */
    left->right = node;                  /**< 将原根 node 变为 left 的右子节点 */
    UpdateHeight(node);                  /**< 更新原根的高度 */
    UpdateHeight(left);                  /**< 更新新根的高度 */
    return left;
}

/**
 * @brief 对以 node 为根的子树执行先左旋后右旋的双旋转（LR 型不平衡）。
 *
 * 适用于平衡因子 > 1 且左子树的平衡因子 < 0 的情况。
 *
 * @param node 待旋转的子树的根节点。
 * @return 旋转后新的子树根节点指针。
 */
static AvlNode *RotateLeftRight(AvlNode *node) {
    node->left = RotateLeft(node->left); /**< 先对左子节点执行左旋 */
    return RotateRight(node);            /**< 再对当前根执行右旋 */
}

/**
 * @brief 对以 node 为根的子树执行先右旋后左旋的双旋转（RL 型不平衡）。
 *
 * 适用于平衡因子 < -1 且右子树的平衡因子 > 0 的情况。
 *
 * @param node 待旋转的子树的根节点。
 * @return 旋转后新的子树根节点指针。
 */
static AvlNode *RotateRightLeft(AvlNode *node) {
    node->right = RotateRight(node->right); /**< 先对右子节点执行右旋 */
    return RotateLeft(node);                /**< 再对当前根执行左旋 */
}

/**
 * @brief 创建一个新的 AVL 树节点，并初始化其数据域和指针域。
 *
 * @param record 指向课程记录的指针，该记录将被拷贝到新节点中。
 * @return 成功时返回新节点的指针；内存分配失败时返回 NULL。
 */
static AvlNode *CreateNode(const CourseRecord *record) {
    AvlNode *node = (AvlNode *)malloc(sizeof(AvlNode));
    if (node == NULL) {
        return NULL;                      /**< 内存分配失败，返回 NULL */
    }
    node->data = *record;                 /**< 拷贝课程记录数据 */
    node->height = 1;                     /**< 新节点初始高度为 1（叶节点） */
    node->left = NULL;                    /**< 左子树初始为空 */
    node->right = NULL;                   /**< 右子树初始为空 */
    return node;
}

/**
 * @brief 递归地向 AVL 子树中插入一条课程记录，并在回溯时进行平衡调整。
 *
 * 比较规则：先按 student_id 字典序升序比较，
 * 若 student_id 相同则按 course_id 字典序升序比较。
 *
 * @param node 当前子树的根节点指针。
 * @param record 待插入的课程记录指针。
 * @return 插入并平衡后子树的根节点指针。
 */
static AvlNode *InsertNode(AvlNode *node, const CourseRecord *record) {
    if (node == NULL) {
        return CreateNode(record);        /**< 到达空位置，创建并插入新节点 */
    }

    /* 按照 (student_id, course_id) 复合键决定插入左子树还是右子树 */
    if (strcmp(record->student_id, node->data.student_id) < 0 ||
        (strcmp(record->student_id, node->data.student_id) == 0 && strcmp(record->course_id, node->data.course_id) < 0)) {
        node->left = InsertNode(node->left, record);
    } else {
        node->right = InsertNode(node->right, record);
    }

    UpdateHeight(node);                   /**< 自底向上更新当前节点高度 */
    int balance = BalanceFactor(node);    /**< 计算当前节点的平衡因子 */

    /* 根据平衡因子判断并执行相应的旋转操作以恢复平衡 */
    if (balance > 1) {
        if (BalanceFactor(node->left) >= 0) {   /**< LL 型：左子树偏左 */
            return RotateRight(node);           /**< 右单旋即可 */
        }
        return RotateLeftRight(node);           /**< LR 型：左子树偏右，需双旋 */
    }
    if (balance < -1) {
        if (BalanceFactor(node->right) <= 0) {  /**< RR 型：右子树偏右 */
            return RotateLeft(node);            /**< 左单旋即可 */
        }
        return RotateRightLeft(node);           /**< RL 型：右子树偏左，需双旋 */
    }

    return node;                          /**< 平衡未被破坏，直接返回 */
}

/**
 * @brief 递归地在 AVL 子树中查找匹配 (student_id, course_id) 的节点。
 *
 * 查找逻辑与插入时的比较规则一致：先比较学号，学号相同再比较课程号。
 *
 * @param node 当前子树的根节点指针。
 * @param student_id 待查找的学号字符串。
 * @param course_id 待查找的课程号字符串。
 * @return 找到则返回对应节点的指针；未找到返回 NULL。
 */
static AvlNode *FindNode(AvlNode *node, const char *student_id, const char *course_id) {
    if (node == NULL) {
        return NULL;                      /**< 到达空节点，查找失败 */
    }

    int cmp_student = strcmp(student_id, node->data.student_id); /**< 学号比较结果 */
    int cmp_course = strcmp(course_id, node->data.course_id);    /**< 课程号比较结果 */

    if (cmp_student == 0 && cmp_course == 0) {
        return node;                      /**< 学号和课程号均匹配，查找成功 */
    }

    /* 根据复合键比较结果决定在左子树还是右子树中继续查找 */
    if (cmp_student < 0 || (cmp_student == 0 && cmp_course < 0)) {
        return FindNode(node->left, student_id, course_id);
    }
    return FindNode(node->right, student_id, course_id);
}

/**
 * @brief 递归地销毁 AVL 子树，释放所有节点的内存。
 *
 * 采用后序遍历方式：先销毁左右子树，再释放当前节点。
 *
 * @param node 待销毁的子树的根节点指针，可为 NULL。
 */
static void DestroyNode(AvlNode *node) {
    if (node == NULL) {
        return;                           /**< 空节点，无需销毁 */
    }
    DestroyNode(node->left);              /**< 递归销毁左子树 */
    DestroyNode(node->right);             /**< 递归销毁右子树 */
    free(node);                           /**< 释放当前节点内存 */
}

/**
 * @brief 中序遍历 AVL 子树，对每个节点的数据调用访问函数 visit。
 *
 * 中序遍历保证了按 (student_id, course_id) 升序访问所有节点。
 *
 * @param node 当前子树的根节点指针。
 * @param visit 函数指针，指向接收 const CourseRecord* 的回调函数。
 */
static void TraverseNode(const AvlNode *node, void (*visit)(const CourseRecord *)) {
    if (node == NULL || visit == NULL) {
        return;                           /**< 空节点或无效回调，直接返回 */
    }
    TraverseNode(node->left, visit);      /**< 递归遍历左子树 */
    visit(&node->data);                   /**< 访问当前节点数据 */
    TraverseNode(node->right, visit);     /**< 递归遍历右子树 */
}

/**
 * @brief 初始化一个 AVL 树，将根指针置空，大小置零。
 *
 * @param tree 指向待初始化的 AVL 树结构体的指针。
 */
void AvlTreeInit(AvlTree *tree) {
    if (tree == NULL) {
        return;                           /**< 空指针保护 */
    }
    tree->root = NULL;                    /**< 根节点初始为空 */
    tree->size = 0;                       /**< 树中节点数初始为 0 */
}

/**
 * @brief 销毁整个 AVL 树，释放所有节点内存并重置树状态。
 *
 * @param tree 指向待销毁的 AVL 树结构体的指针。
 */
void AvlTreeDestroy(AvlTree *tree) {
    if (tree == NULL) {
        return;                           /**< 空指针保护 */
    }
    DestroyNode(tree->root);              /**< 递归销毁所有节点 */
    tree->root = NULL;                    /**< 根指针置空 */
    tree->size = 0;                       /**< 节点计数清零 */
}

/**
 * @brief 向 AVL 树中插入一条课程记录。
 *
 * @param tree 指向 AVL 树结构体的指针。
 * @param record 指向待插入的课程记录的指针。
 * @return 插入成功返回 true；参数无效时返回 false。
 */
bool AvlTreeInsert(AvlTree *tree, const CourseRecord *record) {
    if (tree == NULL || record == NULL) {
        return false;                     /**< 参数校验失败 */
    }
    tree->root = InsertNode(tree->root, record); /**< 执行递归插入并平衡 */
    tree->size++;                         /**< 树规模加 1 */
    return true;
}

/**
 * @brief 从 AVL 树中删除指定 (student_id, course_id) 对应的节点。
 *
 * @note 当前为未实现的占位函数，始终返回 false。
 *
 * @param tree 指向 AVL 树结构体的指针。
 * @param student_id 待删除记录的学号字符串。
 * @param course_id 待删除记录的课程号字符串。
 * @return 当前版本始终返回 false。
 */
bool AvlTreeDelete(AvlTree *tree, const char *student_id, const char *course_id) {
    (void)tree; (void)student_id; (void)course_id; /**< 抑制未使用参数警告 */
    return false;                         /**< 删除操作暂未实现 */
}

/**
 * @brief 在 AVL 树中查找与给定 (student_id, course_id) 匹配的课程记录。
 *
 * @param tree 指向 AVL 树结构体的指针。
 * @param student_id 待查找的学号字符串，不可为 NULL。
 * @param course_id 待查找的课程号字符串，不可为 NULL。
 * @return 找到则返回指向课程记录数据的指针；未找到或参数无效时返回 NULL。
 */
CourseRecord *AvlTreeFind(AvlTree *tree, const char *student_id, const char *course_id) {
    if (tree == NULL || student_id == NULL || course_id == NULL) {
        return NULL;                      /**< 参数校验失败 */
    }
    AvlNode *node = FindNode(tree->root, student_id, course_id);
    return node == NULL ? NULL : &node->data; /**< 返回节点数据指针，或 NULL */
}

/**
 * @brief 中序遍历 AVL 树，对每个节点的记录调用 visit 回调函数。
 *
 * 遍历顺序为按 (student_id, course_id) 复合键升序。
 *
 * @param tree 指向 AVL 树结构体的指针。
 * @param visit 函数指针，指向接收 const CourseRecord* 的回调函数。
 */
void AvlTreeTraverse(const AvlTree *tree, void (*visit)(const CourseRecord *)) {
    if (tree == NULL) {
        return;                           /**< 空指针保护 */
    }
    TraverseNode(tree->root, visit);      /**< 对根节点开始中序遍历 */
}
