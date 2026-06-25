#include "query_processor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int CompareRecords(const void *a, const void *b) {
    const SelectionRecord *ra = (const SelectionRecord *)a;
    const SelectionRecord *rb = (const SelectionRecord *)b;

    if (ra->score != rb->score) {
        return rb->score - ra->score;
    }
    return strcmp(ra->student_id, rb->student_id);
}

void QueryResultInit(QueryResult *result) {
    if (result == NULL) {
        return;
    }
    result->records = NULL;
    result->count = 0;
    result->capacity = 0;
}

void QueryResultFree(QueryResult *result) {
    if (result == NULL) {
        return;
    }
    free(result->records);
    result->records = NULL;
    result->count = 0;
    result->capacity = 0;
}

bool BuildQueryResult(const RecordDatabase *db,
                      const char *course_name,
                      const char *semester,
                      const char *college,
                      int min_score,
                      int max_score,
                      QueryResult *result) {
    if (db == NULL || result == NULL) {
        return false;
    }

    QueryResultInit(result);
    result->records = (SelectionRecord *)calloc(db->count, sizeof(SelectionRecord));
    if (result->records == NULL && db->count > 0) {
        return false;
    }

    result->capacity = db->count;
    for (int i = 0; i < db->count; ++i) {
        const SelectionRecord *record = &db->records[i];
        if (course_name != NULL && course_name[0] != '\0' && strstr(record->course_name, course_name) == NULL) {
            continue;
        }
        if (semester != NULL && semester[0] != '\0' && strcmp(record->semester, semester) != 0) {
            continue;
        }
        if (college != NULL && college[0] != '\0' && strcmp(record->college, college) != 0) {
            continue;
        }
        if (record->score < min_score || record->score > max_score) {
            continue;
        }
        result->records[result->count++] = *record;
    }
    return true;
}

void SortQueryResult(QueryResult *result) {
    if (result == NULL || result->records == NULL || result->count <= 1) {
        return;
    }
    qsort(result->records, result->count, sizeof(SelectionRecord), CompareRecords);
}

void PrintQueryResult(const QueryResult *result) {
    if (result == NULL || result->records == NULL || result->count == 0) {
        puts("无筛选结果。");
        return;
    }

    for (int i = 0; i < result->count; ++i) {
        const SelectionRecord *record = &result->records[i];
        printf("- %s | %s | %s | %d\n", record->student_id, record->student_name, record->course_name, record->score);
    }
}

bool SaveQueryResultToCsv(const QueryResult *result, const char *path) {
    if (result == NULL || path == NULL) {
        return false;
    }

    FILE *fp = fopen(path, "w");
    if (fp == NULL) {
        return false;
    }

    fprintf(fp, "学号,姓名,学院,课程编号,课程名称,学分,选课学期,选课日期,成绩\n");
    for (int i = 0; i < result->count; ++i) {
        const SelectionRecord *record = &result->records[i];
        fprintf(fp, "%s,%s,%s,%s,%s,%.1f,%s,%s,%d\n",
                record->student_id,
                record->student_name,
                record->college,
                record->course_id,
                record->course_name,
                record->credit,
                record->semester,
                record->selected_date,
                record->score);
    }

    fclose(fp);
    return true;
}
