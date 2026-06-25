/**
 * @file record_system.c
 * @brief 选课记录管理模块 - 实现文件
 * @details 该模块提供基础的记录加载、保存、查询、筛选、统计和过期删除功能。
 */

#include "record_system.h"

static bool EnsureCapacity(RecordDatabase *db) {
    if (db->count < db->capacity) {
        return true;
    }

    int new_capacity = db->capacity == 0 ? 128 : db->capacity * 2;
    SelectionRecord *new_records = (SelectionRecord *)realloc(db->records, sizeof(SelectionRecord) * new_capacity);
    if (new_records == NULL) {
        return false;
    }

    db->records = new_records;
    db->capacity = new_capacity;
    return true;
}

static int CompareDate(const char *date1, const char *date2) {
    return strcmp(date1, date2);
}

static void ClearInputBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

void RecordDatabaseInit(RecordDatabase *db) {
    if (db == NULL) {
        return;
    }

    db->records = NULL;
    db->count = 0;
    db->capacity = 0;
}

void RecordDatabaseFree(RecordDatabase *db) {
    if (db == NULL) {
        return;
    }

    free(db->records);
    db->records = NULL;
    db->count = 0;
    db->capacity = 0;
}

bool LoadRecordsFromCsv(RecordDatabase *db, const char *path) {
    if (db == NULL || path == NULL) {
        return false;
    }

    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        return false;
    }

    char line[1024];
    int line_no = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        line_no++;
        if (line_no == 1) {
            continue;
        }

        SelectionRecord record;
        memset(&record, 0, sizeof(record));

        char *token = strtok(line, ",\n\r");
        if (token == NULL) {
            continue;
        }
        snprintf(record.student_id, sizeof(record.student_id), "%s", token);

        token = strtok(NULL, ",\n\r");
        if (token == NULL) {
            continue;
        }
        snprintf(record.student_name, sizeof(record.student_name), "%s", token);

        token = strtok(NULL, ",\n\r");
        if (token == NULL) {
            continue;
        }
        snprintf(record.college, sizeof(record.college), "%s", token);

        token = strtok(NULL, ",\n\r");
        if (token == NULL) {
            continue;
        }
        snprintf(record.course_id, sizeof(record.course_id), "%s", token);

        token = strtok(NULL, ",\n\r");
        if (token == NULL) {
            continue;
        }
        snprintf(record.course_name, sizeof(record.course_name), "%s", token);

        token = strtok(NULL, ",\n\r");
        if (token == NULL) {
            continue;
        }
        record.credit = (float)atof(token);

        token = strtok(NULL, ",\n\r");
        if (token == NULL) {
            continue;
        }
        snprintf(record.semester, sizeof(record.semester), "%s", token);

        token = strtok(NULL, ",\n\r");
        if (token == NULL) {
            continue;
        }
        snprintf(record.selected_date, sizeof(record.selected_date), "%s", token);

        token = strtok(NULL, ",\n\r");
        if (token == NULL) {
            continue;
        }
        record.score = atoi(token);

        if (!EnsureCapacity(db)) {
            fclose(fp);
            return false;
        }

        db->records[db->count++] = record;
    }

    fclose(fp);
    return true;
}

bool SaveRecordsToCsv(const RecordDatabase *db, const char *path) {
    if (db == NULL || path == NULL) {
        return false;
    }

    FILE *fp = fopen(path, "w");
    if (fp == NULL) {
        return false;
    }

    fprintf(fp, "学号,姓名,学院,课程编号,课程名称,学分,选课学期,选课日期,成绩\n");
    for (int i = 0; i < db->count; ++i) {
        const SelectionRecord *record = &db->records[i];
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

bool InsertRecord(RecordDatabase *db, const SelectionRecord *record) {
    if (db == NULL || record == NULL) {
        return false;
    }

    if (!EnsureCapacity(db)) {
        return false;
    }

    db->records[db->count++] = *record;
    return true;
}

bool DeleteRecordByKey(RecordDatabase *db, const char *student_id, const char *course_id) {
    if (db == NULL || student_id == NULL || course_id == NULL) {
        return false;
    }

    for (int i = 0; i < db->count; ++i) {
        if (strcmp(db->records[i].student_id, student_id) == 0 && strcmp(db->records[i].course_id, course_id) == 0) {
            for (int j = i; j + 1 < db->count; ++j) {
                db->records[j] = db->records[j + 1];
            }
            db->count--;
            return true;
        }
    }

    return false;
}

bool UpdateRecordScore(RecordDatabase *db, const char *student_id, const char *course_id, int new_score) {
    if (db == NULL || student_id == NULL || course_id == NULL) {
        return false;
    }

    for (int i = 0; i < db->count; ++i) {
        if (strcmp(db->records[i].student_id, student_id) == 0 && strcmp(db->records[i].course_id, course_id) == 0) {
            db->records[i].score = new_score;
            return true;
        }
    }

    return false;
}

SelectionRecord *FindRecordByKey(RecordDatabase *db, const char *student_id, const char *course_id) {
    if (db == NULL || student_id == NULL || course_id == NULL) {
        return NULL;
    }

    for (int i = 0; i < db->count; ++i) {
        if (strcmp(db->records[i].student_id, student_id) == 0 && strcmp(db->records[i].course_id, course_id) == 0) {
            return &db->records[i];
        }
    }

    return NULL;
}

void PrintAllRecords(const RecordDatabase *db) {
    if (db == NULL || db->count == 0) {
        puts("当前没有任何记录。");
        return;
    }

    printf("共 %d 条记录：\n", db->count);
    for (int i = 0; i < db->count; ++i) {
        const SelectionRecord *record = &db->records[i];
        printf("- 学号:%s | 姓名:%s | 课程:%s | 成绩:%d\n",
               record->student_id,
               record->student_name,
               record->course_name,
               record->score);
    }
}

void ShowStatistics(const RecordDatabase *db) {
    if (db == NULL || db->count == 0) {
        puts("当前没有记录可统计。");
        return;
    }

    int total_students = 0;
    int total_courses = 0;
    int total_score = 0;
    int passed_count = 0;

    for (int i = 0; i < db->count; ++i) {
        const SelectionRecord *record = &db->records[i];
        total_students += 1;
        total_courses += 1;
        total_score += record->score;
        if (record->score >= 60) {
            passed_count++;
        }
    }

    printf("记录总数：%d\n", db->count);
    printf("总学生数：%d\n", total_students);
    printf("总课程数：%d\n", total_courses);
    printf("平均成绩：%.2f\n", (double)total_score / db->count);
    printf("及格人数：%d\n", passed_count);
}

void FilterAndPrint(const RecordDatabase *db, const char *course_name, const char *semester, const char *college, int min_score, int max_score) {
    if (db == NULL) {
        return;
    }

    int matched = 0;
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

        matched++;
        printf("- 学号:%s | 姓名:%s | 课程:%s | 成绩:%d\n",
               record->student_id,
               record->student_name,
               record->course_name,
               record->score);
    }

    printf("筛选结果共 %d 条。\n", matched);
}

int DeleteExpiredRecords(RecordDatabase *db, const char *base_date) {
    if (db == NULL || base_date == NULL) {
        return 0;
    }

    int removed = 0;
    int write_index = 0;

    for (int i = 0; i < db->count; ++i) {
        if (CompareDate(db->records[i].selected_date, base_date) < 0) {
            removed++;
        } else {
            db->records[write_index++] = db->records[i];
        }
    }

    db->count = write_index;
    return removed;
}

