/**
 * @file record_system.c
 * @brief 选课记录管理模块 - 实现文件
 * @details 该模块提供选课记录的加载、保存、增删改查、筛选、统计以及过期删除等核心功能。
 *          所有记录以动态数组形式存储在 RecordDatabase 结构中，支持 CSV 格式的导入与导出。
 */

#include "record_system.h"

/**
 * @brief 确保数据库容量足够容纳新记录，不足时自动扩容
 * @details 当 records 数组已满时，容量翻倍（首次分配默认 128 条）。
 *          使用 realloc 在原地扩展或重新分配内存。
 * @param Database 指向待操作的记录数据库指针
 * @return true  容量足够或扩容成功
 * @return false 内存分配失败
 */
static bool EnsureCapacity(RecordDatabase *Database) {
    if (Database->count < Database->capacity) {
        return true;
    }

    /// 新的目标容量：首次分配为 128，后续为当前容量的 2 倍
    int NewCapacity = Database->capacity == 0 ? 128 : Database->capacity * 2;
    SelectionRecord *NewRecords = (SelectionRecord *)realloc(Database->records, sizeof(SelectionRecord) * NewCapacity);
    if (NewRecords == NULL) {
        return false;
    }

    Database->records = NewRecords;
    Database->capacity = NewCapacity;
    return true;
}

/**
 * @brief 比较两个日期字符串的大小（按字典序）
 * @param Date1 第一个日期字符串（格式 YYYY-MM-DD 或类似）
 * @param Date2 第二个日期字符串
 * @return <0  Date1 早于 Date2
 * @return 0   Date1 等于 Date2
 * @return >0  Date1 晚于 Date2
 */
static int CompareDate(const char *Date1, const char *Date2) {
    return strcmp(Date1, Date2);
}

/**
 * @brief 初始化记录数据库
 * @details 将 records 指针置空，count 与 capacity 清零，
 *          为后续加载或插入记录做好准备。
 * @param Database 指向待初始化的 RecordDatabase 结构体
 */
void RecordDatabaseInit(RecordDatabase *Database) {
    if (Database == NULL) {
        return;
    }

    Database->records = NULL;   ///< 选课记录动态数组指针
    Database->count = 0;        ///< 当前已存储的记录条数
    Database->capacity = 0;     ///< 动态数组当前分配的最大容量
}

/**
 * @brief 释放记录数据库所占用的内存
 * @details 释放 records 动态数组，并将指针置空、计数清零。
 * @param Database 指向待释放的 RecordDatabase 结构体
 */
void RecordDatabaseFree(RecordDatabase *Database) {
    if (Database == NULL) {
        return;
    }

    free(Database->records);
    Database->records = NULL;
    Database->count = 0;
    Database->capacity = 0;
}

/**
 * @brief 从 CSV 文件中加载选课记录到数据库
 * @details 按行读取 CSV 文件，第一行为表头自动跳过，
 *          后续每一行解析为一条 SelectionRecord。
 *          CSV 列顺序：学号,姓名,学院,课程编号,课程名称,学分,选课学期,选课日期,成绩
 *          解析过程中自动调用 EnsureCapacity 保证容量充足。
 * @param Database 指向目标记录数据库
 * @param Path     CSV 文件的路径字符串
 * @return true  加载成功
 * @return false 文件打开失败、格式错误或内存不足
 */
bool LoadRecordsFromCsv(RecordDatabase *Database, const char *Path) {
    if (Database == NULL || Path == NULL) {
        return false;
    }

    FILE *FilePointer = fopen(Path, "r");  ///< 以只读模式打开 CSV 文件
    if (FilePointer == NULL) {
        return false;
    }

    char Line[1024];    ///< 行缓冲区，存储当前读取的一行文本
    int LineNo = 0;     ///< 当前行号，用于跳过表头（第 1 行）

    while (fgets(Line, sizeof(Line), FilePointer) != NULL) {
        LineNo++;
        if (LineNo == 1) {
            continue;   ///< 跳过 CSV 表头行
        }

        SelectionRecord Record;
        memset(&Record, 0, sizeof(Record));

        /// 按逗号（,）和换行符（\n \r）逐字段解析
        char *Token = strtok(Line, ",\n\r");  ///< 当前解析到的字段内容
        if (Token == NULL) {
            continue;
        }
        snprintf(Record.student_id, sizeof(Record.student_id), "%s", Token);   ///< 学号

        Token = strtok(NULL, ",\n\r");
        if (Token == NULL) {
            continue;
        }
        snprintf(Record.student_name, sizeof(Record.student_name), "%s", Token); ///< 姓名

        Token = strtok(NULL, ",\n\r");
        if (Token == NULL) {
            continue;
        }
        snprintf(Record.college, sizeof(Record.college), "%s", Token);           ///< 学院

        Token = strtok(NULL, ",\n\r");
        if (Token == NULL) {
            continue;
        }
        snprintf(Record.course_id, sizeof(Record.course_id), "%s", Token);       ///< 课程编号

        Token = strtok(NULL, ",\n\r");
        if (Token == NULL) {
            continue;
        }
        snprintf(Record.course_name, sizeof(Record.course_name), "%s", Token);   ///< 课程名称

        Token = strtok(NULL, ",\n\r");
        if (Token == NULL) {
            continue;
        }
        Record.credit = (float)atof(Token);  ///< 学分（浮点数）

        Token = strtok(NULL, ",\n\r");
        if (Token == NULL) {
            continue;
        }
        snprintf(Record.semester, sizeof(Record.semester), "%s", Token);         ///< 选课学期

        Token = strtok(NULL, ",\n\r");
        if (Token == NULL) {
            continue;
        }
        snprintf(Record.selected_date, sizeof(Record.selected_date), "%s", Token); ///< 选课日期

        Token = strtok(NULL, ",\n\r");
        if (Token == NULL) {
            continue;
        }
        Record.score = atoi(Token);  ///< 成绩（整数）

        if (!EnsureCapacity(Database)) {
            fclose(FilePointer);
            return false;
        }

        Database->records[Database->count++] = Record;
    }

    fclose(FilePointer);
    return true;
}

/**
 * @brief 将数据库中的所有记录保存为 CSV 文件
 * @details 写入 UTF-8 编码的 CSV 文件，第一行为中文表头，
 *          后续每行对应一条记录，字段之间以逗号分隔。
 * @param Database 指向只读的记录数据库
 * @param Path     目标 CSV 文件路径
 * @return true  保存成功
 * @return false 参数无效或文件无法创建
 */
bool SaveRecordsToCsv(const RecordDatabase *Database, const char *Path) {
    if (Database == NULL || Path == NULL) {
        return false;
    }

    FILE *FilePointer = fopen(Path, "w");  ///< 以写入模式打开文件
    if (FilePointer == NULL) {
        return false;
    }

    fprintf(FilePointer, "学号,姓名,学院,课程编号,课程名称,学分,选课学期,选课日期,成绩\n");
    for (int Index = 0; Index < Database->count; ++Index) {
        const SelectionRecord *Record = &Database->records[Index];  ///< 当前记录指针
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

/**
 * @brief 向数据库末尾插入一条选课记录
 * @details 自动扩容后将源记录深拷贝到数组末尾，count 自增。
 * @param Database 指向目标记录数据库
 * @param Record   指向待插入的选课记录（深拷贝源）
 * @return true  插入成功
 * @return false 参数无效或扩容失败
 */
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

/**
 * @brief 根据学号与课程编号删除一条选课记录
 * @details 线性查找匹配记录，找到后将后续元素前移覆盖，count 减 1。
 * @param Database  指向目标记录数据库
 * @param StudentId 待删除记录的学生学号
 * @param CourseId  待删除记录的课程编号
 * @return true  找到并成功删除
 * @return false 未找到匹配记录或参数无效
 */
bool DeleteRecordByKey(RecordDatabase *Database, const char *StudentId, const char *CourseId) {
    if (Database == NULL || StudentId == NULL || CourseId == NULL) {
        return false;
    }

    for (int Index = 0; Index < Database->count; ++Index) {
        if (strcmp(Database->records[Index].student_id, StudentId) == 0 && strcmp(Database->records[Index].course_id, CourseId) == 0) {
            /// ShiftIndex：将 Index 之后的所有元素向前移动一位
            for (int ShiftIndex = Index; ShiftIndex + 1 < Database->count; ++ShiftIndex) {
                Database->records[ShiftIndex] = Database->records[ShiftIndex + 1];
            }
            Database->count--;
            return true;
        }
    }

    return false;
}

/**
 * @brief 根据学号与课程编号更新成绩
 * @details 线性查找匹配记录，直接修改其 score 字段。
 * @param Database  指向目标记录数据库
 * @param StudentId 待更新记录的学生学号
 * @param CourseId  待更新记录的课程编号
 * @param NewScore  新的成绩值（整数）
 * @return true  找到并成功更新
 * @return false 未找到匹配记录或参数无效
 */
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

/**
 * @brief 根据学号与课程编号查找选课记录
 * @details 线性搜索 records 数组，返回第一条匹配记录的指针。
 *          返回的指针指向数据库内部内存，调用者不应释放。
 * @param Database  指向记录数据库
 * @param StudentId 待查找的学生学号
 * @param CourseId  待查找的课程编号
 * @return 指向匹配的 SelectionRecord 的指针，若未找到或参数无效则返回 NULL
 */
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

/**
 * @brief 打印数据库中所有记录到标准输出
 * @details 若数据库为空则输出提示信息，否则逐条打印学号、姓名、课程名和成绩。
 * @param Database 指向只读的记录数据库
 */
void PrintAllRecords(const RecordDatabase *Database) {
    if (Database == NULL || Database->count == 0) {
        puts("当前没有任何记录。");
        return;
    }

    printf("共 %d 条记录：\n", Database->count);
    for (int Index = 0; Index < Database->count; ++Index) {
        const SelectionRecord *Record = &Database->records[Index];  ///< 当前遍历到的记录
        printf("- 学号:%s | 姓名:%s | 课程:%s | 成绩:%d\n",
               Record->student_id,
               Record->student_name,
               Record->course_name,
               Record->score);
    }
}

/**
 * @brief 统计并输出数据库的汇总信息
 * @details 计算记录总数、总学生数、总课程数、平均成绩及及格人数并打印。
 * @param db 指向只读的记录数据库
 */
void ShowStatistics(const RecordDatabase *db) {
    if (db == NULL || db->count == 0) {
        puts("当前没有记录可统计。");
        return;
    }

    int total_students = 0;   ///< 累计学生人数
    int total_courses = 0;    ///< 累计课程门数
    int total_score = 0;      ///< 成绩总和，用于计算平均分
    int passed_count = 0;     ///< 成绩 ≥ 60 的记录数

    for (int i = 0; i < db->count; ++i) {
        const SelectionRecord *record = &db->records[i];  ///< 当前遍历到的记录
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

/**
 * @brief 按条件筛选记录并打印
 * @details 支持按课程名称（子串匹配）、学期（精确匹配）、学院（精确匹配）、
 *          成绩区间 [min_score, max_score] 进行组合筛选。
 *          空字符串或 NULL 的条件视为不限制。
 * @param db          指向只读的记录数据库
 * @param course_name 课程名称筛选关键字（子串匹配），可为 NULL 或空串表示不限制
 * @param semester    学期筛选条件（精确匹配），可为 NULL 或空串表示不限制
 * @param college     学院筛选条件（精确匹配），可为 NULL 或空串表示不限制
 * @param min_score   成绩下限（含）
 * @param max_score   成绩上限（含）
 */
void FilterAndPrint(const RecordDatabase *db, const char *course_name, const char *semester, const char *college, int min_score, int max_score) {
    if (db == NULL) {
        return;
    }

    int matched = 0;  ///< 满足筛选条件的记录计数

    for (int i = 0; i < db->count; ++i) {
        const SelectionRecord *record = &db->records[i];  ///< 当前遍历到的记录
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

/**
 * @brief 删除选课日期早于指定日期的过期记录
 * @details 遍历数据库，保留 selected_date >= base_date 的记录，
 *          被删除的记录数由返回值给出。比较基于字符串字典序。
 * @param db        指向待操作的记录数据库
 * @param base_date 基准日期字符串（格式 YYYY-MM-DD 或类似），早于此日期的记录将被移除
 * @return 被删除的记录条数；参数无效时返回 0
 */
int DeleteExpiredRecords(RecordDatabase *db, const char *base_date) {
    if (db == NULL || base_date == NULL) {
        return 0;
    }

    int removed = 0;      ///< 已删除的记录计数
    int write_index = 0;  ///< 写入指针，指向下一个应保留记录要覆盖的位置

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

