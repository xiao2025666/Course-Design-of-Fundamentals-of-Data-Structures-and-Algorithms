#include "statistics.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 输出当前数据库中的记录条数。
 *
 * @param Database 指向记录数据库的指针，其 count 字段表示已存储的记录数量。
 * @note  若 Database 为 NULL 空指针，函数直接返回以避免解引用崩溃。
 */
void StatCourseCount(const RecordDatabase *Database) {
    /* 空指针保护：避免对 NULL 解引用导致段错误 */
    if (Database == NULL) {
        return;
    }
    printf("当前记录条数：%d\n", Database->count);
}

/**
 * @brief 统计学生选课总学分，遍历全部记录累加每门课的学分。
 *
 * @param Database 指向记录数据库的指针。
 *                 records[].credit —— 每门课程对应的学分（float 类型）。
 * @note  若数据库为空或无记录则直接返回，不输出任何内容。
 *        总分保留一位小数输出，以 %.1f 格式化。
 */
void StatStudentCredits(const RecordDatabase *Database) {
    /* 空库或无记录时无需统计，直接返回 */
    if (Database == NULL || Database->count == 0) {
        return;
    }

    float TotalCredit = 0.0f;  /* 累加器，存储所有课程学分的总和 */
    for (int Index = 0; Index < Database->count; ++Index) {
        TotalCredit += Database->records[Index].credit;
    }
    printf("总学分：%.1f\n", TotalCredit);
}

/**
 * @brief 统计"计算机科学与工程学院"在数据库中的记录条数，
 *        用于了解某一学院的选课分布情况。
 *
 * @param Database 指向记录数据库的指针。
 *                 records[].college —— 开课学院名称字符串，用于 strcmp 精确匹配。
 * @note  当前版本硬编码了学院名称为"计算机科学与工程学院"，
 *         如需统计其他学院可修改字符串常量。
 */
void StatCollegeDistribution(const RecordDatabase *Database) {
    /* 空库或无记录时无需统计 */
    if (Database == NULL || Database->count == 0) {
        return;
    }

    int Count = 0;  /* 匹配"计算机科学与工程学院"的记录计数器 */
    for (int Index = 0; Index < Database->count; ++Index) {
        /* strcmp 返回 0 表示两字符串完全相等 */
        if (strcmp(Database->records[Index].college, "计算机科学与工程学院") == 0) {
            Count++;
        }
    }
    printf("计算机学院记录数：%d\n", Count);
}

/**
 * @brief 统计不同学期的记录热度，将包含"2023"的学期视为热门学期。
 *
 * @param Database 指向记录数据库的指针。
 *                 records[].semester —— 学期字符串（如"2023-2024-1"），
 *                 使用 strstr 做子串匹配，只要包含"2023"即计入。
 *                 SpringCount —— 匹配"2023"的记录数。
 *                 其余记录数 = 总记录数 - SpringCount。
 * @note  strstr 返回非 NULL 表示 semester 中包含子串"2023"。
 *        该变量名 SpringCount 为历史命名，实际泛指 2023 年相关学期。
 */
void StatSemesterHotness(const RecordDatabase *Database) {
    /* 空库或无记录时无需统计 */
    if (Database == NULL || Database->count == 0) {
        return;
    }

    int SpringCount = 0;  /* 学期字段中包含"2023"的记录条数 */
    for (int Index = 0; Index < Database->count; ++Index) {
        /* strstr 在 semester 中查找子串"2023"，找不到则返回 NULL */
        if (strstr(Database->records[Index].semester, "2023") != NULL) {
            SpringCount++;
        }
    }
    printf("2023 学期记录数：%d\n", SpringCount);
    printf("其余学期记录数：%d\n", Database->count - SpringCount);
}

/**
 * @brief 按成绩段统计优良中差分布，将成绩划分为三个档次。
 *
 * @param Database 指向记录数据库的指针。
 *                 records[].score —— 课程成绩（数值类型）。
 *                 分段规则：
 *                   - 优秀 (ExcellentCount): score >= 90
 *                   - 及格 (PassCount):      60 <= score < 90
 *                   - 不及格 (FailCount):     score < 60
 * @note  分段边界值 90 和 60 为常见教学管理标准，可根据需要调整。
 *        三个计数器互斥，每条记录仅落入一个分段。
 */
void StatScoreSegments(const RecordDatabase *Database) {
    /* 空库或无记录时无需统计 */
    if (Database == NULL || Database->count == 0) {
        return;
    }

    int ExcellentCount = 0;  /* 优秀：成绩 >= 90 */
    int PassCount = 0;       /* 及格：60 <= 成绩 < 90 */
    int FailCount = 0;       /* 不及格：成绩 < 60 */

    for (int Index = 0; Index < Database->count; ++Index) {
        if (Database->records[Index].score >= 90) {
            ExcellentCount++;
        } else if (Database->records[Index].score >= 60) {
            PassCount++;
        } else {
            FailCount++;
        }
    }
    printf("优良中差分段：优秀=%d, 及格=%d, 不及格=%d\n", ExcellentCount, PassCount, FailCount);
}
