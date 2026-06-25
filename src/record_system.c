/**
 * @file record_system.c
 * @brief 选课记录管理模块 - 实现文件
 * @details 该模块提供基础的记录加载、保存、查询、筛选、统计和过期删除功能。
 */

#include "record_system.h"

static bool EnsureCapacity(RecordDatabase *Database) {
    if (Database->count < Database->capacity) {
        return true;
    }

    int NewCapacity = Database->capacity == 0 ? 128 : Database->capacity * 2;
    SelectionRecord *NewRecords = (SelectionRecord *)realloc(Database->records, sizeof(SelectionRecord) * NewCapacity);
    if (NewRecords == NULL) {
        return false;
    }

    Database->records = NewRecords;
    Database->capacity = NewCapacity;
    return true;
}

static int CompareDate(const char *Date1, const char *Date2) {
    return strcmp(Date1, Date2);
}

void RecordDatabaseInit(RecordDatabase *Database) {
    if (Database == NULL) {
        return;
    }

    Database->records = NULL;
    Database->count = 0;
    Database->capacity = 0;
}

void RecordDatabaseFree(RecordDatabase *Database) {
    if (Database == NULL) {
        return;
    }

    free(Database->records);
    Database->records = NULL;
    Database->count = 0;
    Database->capacity = 0;
}

bool LoadRecordsFromCsv(RecordDatabase *Database, const char *Path) {
    if (Database == NULL || Path == NULL) {
        return false;
    }

    FILE *FilePointer = fopen(Path, "r");
    if (FilePointer == NULL) {
        return false;
    }

    char Line[1024];
    int LineNo = 0;

    while (fgets(Line, sizeof(Line), FilePointer) != NULL) {
        LineNo++;
        if (LineNo == 1) {
            continue;
        }

        SelectionRecord Record;
        memset(&Record, 0, sizeof(Record));

        char *Token = strtok(Line, ",\n\r");
        if (Token == NULL) {
            continue;
        }
        snprintf(Record.student_id, sizeof(Record.student_id), "%s", Token);

        Token = strtok(NULL, ",\n\r");
        if (Token == NULL) {
            continue;
        }
        snprintf(Record.student_name, sizeof(Record.student_name), "%s", Token);

        Token = strtok(NULL, ",\n\r");
        if (Token == NULL) {
            continue;
        }
        snprintf(Record.college, sizeof(Record.college), "%s", Token);

        Token = strtok(NULL, ",\n\r");
        if (Token == NULL) {
            continue;
        }
        snprintf(Record.course_id, sizeof(Record.course_id), "%s", Token);

        Token = strtok(NULL, ",\n\r");
        if (Token == NULL) {
            continue;
        }
        snprintf(Record.course_name, sizeof(Record.course_name), "%s", Token);

        Token = strtok(NULL, ",\n\r");
        if (Token == NULL) {
            continue;
        }
        Record.credit = (float)atof(Token);

        Token = strtok(NULL, ",\n\r");
        if (Token == NULL) {
            continue;
        }
        snprintf(Record.semester, sizeof(Record.semester), "%s", Token);

        Token = strtok(NULL, ",\n\r");
        if (Token == NULL) {
            continue;
        }
        snprintf(Record.selected_date, sizeof(Record.selected_date), "%s", Token);

        Token = strtok(NULL, ",\n\r");
        if (Token == NULL) {
            continue;
        }
        Record.score = atoi(Token);

        if (!EnsureCapacity(Database)) {
            fclose(FilePointer);
            return false;
        }

        Database->records[Database->count++] = Record;
    }

    fclose(FilePointer);
    return true;
}

bool SaveRecordsToCsv(const RecordDatabase *Database, const char *Path) {
    if (Database == NULL || Path == NULL) {
        return false;
    }

    FILE *FilePointer = fopen(Path, "w");
    if (FilePointer == NULL) {
        return false;
    }

    fprintf(FilePointer, "学号,姓名,学院,课程编号,课程名称,学分,选课学期,选课日期,成绩\n");
    for (int Index = 0; Index < Database->count; ++Index) {
        const SelectionRecord *Record = &Database->records[Index];
        fprintf(FilePointer, "%s,%s,%s,%s,%s,%.1f,%s,%s,%d\n",
                Record->student_id,
                Record->student_name,
                Record->college,
                Record->course_id,
                Record->course_name,
                Record->credit,
                Record->semester,
                Record->selected_date,
                Record->score);
    }

    fclose(FilePointer);
    return true;
}

bool InsertRecord(RecordDatabase *Database, const SelectionRecord *Record) {
    if (Database == NULL || Record == NULL) {
        return false;
    }

    if (!EnsureCapacity(Database)) {
        return false;
    }

    Database->records[Database->count++] = *Record;
    return true;
}

bool DeleteRecordByKey(RecordDatabase *Database, const char *StudentId, const char *CourseId) {
    if (Database == NULL || StudentId == NULL || CourseId == NULL) {
        return false;
    }

    for (int Index = 0; Index < Database->count; ++Index) {
        if (strcmp(Database->records[Index].student_id, StudentId) == 0 && strcmp(Database->records[Index].course_id, CourseId) == 0) {
            for (int ShiftIndex = Index; ShiftIndex + 1 < Database->count; ++ShiftIndex) {
                Database->records[ShiftIndex] = Database->records[ShiftIndex + 1];
            }
            Database->count--;
            return true;
        }
    }

    return false;
}

bool UpdateRecordScore(RecordDatabase *Database, const char *StudentId, const char *CourseId, int NewScore) {
    if (Database == NULL || StudentId == NULL || CourseId == NULL) {
        return false;
    }

    for (int Index = 0; Index < Database->count; ++Index) {
        if (strcmp(Database->records[Index].student_id, StudentId) == 0 && strcmp(Database->records[Index].course_id, CourseId) == 0) {
            Database->records[Index].score = NewScore;
            return true;
        }
    }

    return false;
}

SelectionRecord *FindRecordByKey(RecordDatabase *Database, const char *StudentId, const char *CourseId) {
    if (Database == NULL || StudentId == NULL || CourseId == NULL) {
        return NULL;
    }

    for (int Index = 0; Index < Database->count; ++Index) {
        if (strcmp(Database->records[Index].student_id, StudentId) == 0 && strcmp(Database->records[Index].course_id, CourseId) == 0) {
            return &Database->records[Index];
        }
    }

    return NULL;
}

void PrintAllRecords(const RecordDatabase *Database) {
    if (Database == NULL || Database->count == 0) {
        puts("当前没有任何记录。");
        return;
    }

    printf("共 %d 条记录：\n", Database->count);
    for (int Index = 0; Index < Database->count; ++Index) {
        const SelectionRecord *Record = &Database->records[Index];
        printf("- 学号:%s | 姓名:%s | 课程:%s | 成绩:%d\n",
               Record->student_id,
               Record->student_name,
               Record->course_name,
               Record->score);
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

