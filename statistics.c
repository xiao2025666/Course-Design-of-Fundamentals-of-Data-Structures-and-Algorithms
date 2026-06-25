#include "statistics.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void StatCourseCount(const RecordDatabase *db) {
    if (db == NULL) {
        return;
    }
    printf("当前记录条数：%d\n", db->count);
}

void StatStudentCredits(const RecordDatabase *db) {
    if (db == NULL || db->count == 0) {
        return;
    }

    float total_credit = 0.0f;
    for (int i = 0; i < db->count; ++i) {
        total_credit += db->records[i].credit;
    }
    printf("总学分：%.1f\n", total_credit);
}

void StatCollegeDistribution(const RecordDatabase *db) {
    if (db == NULL || db->count == 0) {
        return;
    }

    int counts = 0;
    for (int i = 0; i < db->count; ++i) {
        if (strcmp(db->records[i].college, "计算机科学与工程学院") == 0) {
            counts++;
        }
    }
    printf("计算机学院记录数：%d\n", counts);
}

void StatSemesterHotness(const RecordDatabase *db) {
    if (db == NULL || db->count == 0) {
        return;
    }

    int spring = 0;
    int autumn = 0;
    for (int i = 0; i < db->count; ++i) {
        if (strstr(db->records[i].semester, "2023") != NULL) {
            spring++;
        }
    }
    printf("2023 学期记录数：%d\n", spring);
    printf("其余学期记录数：%d\n", db->count - spring);
}

void StatScoreSegments(const RecordDatabase *db) {
    if (db == NULL || db->count == 0) {
        return;
    }

    int excellent = 0, pass = 0, fail = 0;
    for (int i = 0; i < db->count; ++i) {
        if (db->records[i].score >= 90) {
            excellent++;
        } else if (db->records[i].score >= 60) {
            pass++;
        } else {
            fail++;
        }
    }
    printf("优良中差分段：优秀=%d, 及格=%d, 不及格=%d\n", excellent, pass, fail);
}
