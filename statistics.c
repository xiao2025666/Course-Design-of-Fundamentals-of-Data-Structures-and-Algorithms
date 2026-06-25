#include "statistics.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 输出当前数据库中的记录条数。
 */
void StatCourseCount(const RecordDatabase *Database) {
    if (Database == NULL) {
        return;
    }
    printf("当前记录条数：%d\n", Database->count);
}

/**
 * @brief 统计学生选课总学分。
 */
void StatStudentCredits(const RecordDatabase *Database) {
    if (Database == NULL || Database->count == 0) {
        return;
    }

    float TotalCredit = 0.0f;
    for (int Index = 0; Index < Database->count; ++Index) {
        TotalCredit += Database->records[Index].credit;
    }
    printf("总学分：%.1f\n", TotalCredit);
}

/**
 * @brief 统计某学院的记录分布情况。
 */
void StatCollegeDistribution(const RecordDatabase *Database) {
    if (Database == NULL || Database->count == 0) {
        return;
    }

    int Count = 0;
    for (int Index = 0; Index < Database->count; ++Index) {
        if (strcmp(Database->records[Index].college, "计算机科学与工程学院") == 0) {
            Count++;
        }
    }
    printf("计算机学院记录数：%d\n", Count);
}

/**
 * @brief 统计不同学期的记录热度。
 */
void StatSemesterHotness(const RecordDatabase *Database) {
    if (Database == NULL || Database->count == 0) {
        return;
    }

    int SpringCount = 0;
    for (int Index = 0; Index < Database->count; ++Index) {
        if (strstr(Database->records[Index].semester, "2023") != NULL) {
            SpringCount++;
        }
    }
    printf("2023 学期记录数：%d\n", SpringCount);
    printf("其余学期记录数：%d\n", Database->count - SpringCount);
}

/**
 * @brief 按成绩段统计优良中差分布。
 */
void StatScoreSegments(const RecordDatabase *Database) {
    if (Database == NULL || Database->count == 0) {
        return;
    }

    int ExcellentCount = 0, PassCount = 0, FailCount = 0;
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
