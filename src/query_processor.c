#include "query_processor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 比较两个选课记录的排序优先级。
 * @details 该比较函数供 qsort 使用，优先按成绩从高到低排序；
 * 当成绩相同时，再按学号字符串升序排列，以保证结果具有稳定可读性。
 * @param a 指向第一个记录的指针。
 * @param b 指向第二个记录的指针。
 * @return 若 a 应排在 b 前面，返回负值；反之返回正值；相等返回 0。
 */
static int CompareRecords(const void *FirstItem, const void *SecondItem) {
    const SelectionRecord *FirstRecord = (const SelectionRecord *)FirstItem;
    const SelectionRecord *SecondRecord = (const SelectionRecord *)SecondItem;

    if (FirstRecord->score != SecondRecord->score) {
        return SecondRecord->score - FirstRecord->score;
    }
    return strcmp(FirstRecord->student_id, SecondRecord->student_id);
}

/**
 * @brief 初始化查询结果容器。
 * @details 将查询结果结构体中的指针、计数和容量字段清零，确保后续
 * 追加结果时不会访问到未初始化的内存。
 * @param result 待初始化的查询结果对象。
 */
void QueryResultInit(QueryResult *Result) {
    if (Result == NULL) {
        return;
    }
    Result->Records = NULL;
    Result->Count = 0;
    Result->Capacity = 0;
}

/**
 * @brief 释放查询结果占用的内存。
 * @details 释放记录数组，并将结果容器恢复到初始状态，避免内存泄漏。
 * @param result 需要释放的查询结果对象。
 */
void QueryResultFree(QueryResult *Result) {
    if (Result == NULL) {
        return;
    }
    free(Result->Records);
    Result->Records = NULL;
    Result->Count = 0;
    Result->Capacity = 0;
}

/**
 * @brief 根据多个条件生成筛选结果。
 * @details 从数据库中遍历每条记录，依次检查课程名称关键字、学期、学院和成绩范围。
 * 满足所有条件的记录会被复制到查询结果数组中，便于后续显示或导出。
 * @param db 源数据库对象。
 * @param course_name 课程名称关键词，允许为空。
 * @param semester 学期过滤条件，允许为空。
 * @param college 学院过滤条件，允许为空。
 * @param min_score 最低成绩下限。
 * @param max_score 最高成绩上限。
 * @param result 输出参数，用于保存筛选后的结果。
 * @return 若成功构造结果返回 true，否则返回 false。
 */
bool BuildQueryResult(const RecordDatabase *Db,
                      const char *CourseName,
                      const char *Semester,
                      const char *College,
                      int MinScore,
                      int MaxScore,
                      QueryResult *Result) {
    if (Db == NULL || Result == NULL) {
        return false;
    }

    QueryResultInit(Result);
    Result->Records = (SelectionRecord *)calloc(Db->count, sizeof(SelectionRecord));
    if (Result->Records == NULL && Db->count > 0) {
        return false;
    }

    Result->Capacity = Db->count;
    for (int Index = 0; Index < Db->count; ++Index) {
        const SelectionRecord *CurrentRecord = &Db->records[Index];
        if (CourseName != NULL && CourseName[0] != '\0' && strstr(CurrentRecord->course_name, CourseName) == NULL) {
            continue;
        }
        if (Semester != NULL && Semester[0] != '\0' && strcmp(CurrentRecord->semester, Semester) != 0) {
            continue;
        }
        if (College != NULL && College[0] != '\0' && strcmp(CurrentRecord->college, College) != 0) {
            continue;
        }
        if (CurrentRecord->score < MinScore || CurrentRecord->score > MaxScore) {
            continue;
        }
        Result->Records[Result->Count++] = *CurrentRecord;
    }
    return true;
}

/**
 * @brief 对筛选结果执行排序。
 * @details 使用快速排序算法按照成绩降序、学号升序对当前结果集进行重排，
 * 使输出结果更符合用户阅读习惯。
 * @param result 需要排序的查询结果对象。
 */
void SortQueryResult(QueryResult *Result) {
    if (Result == NULL || Result->Records == NULL || Result->Count <= 1) {
        return;
    }
    qsort(Result->Records, Result->Count, sizeof(SelectionRecord), CompareRecords);
}

/**
 * @brief 打印查询结果。
 * @details 将筛选后的记录逐条输出到终端，便于用户直接查看查询效果。
 * @param result 待打印的查询结果对象。
 */
void PrintQueryResult(const QueryResult *Result) {
    if (Result == NULL || Result->Records == NULL || Result->Count == 0) {
        puts("无筛选结果。");
        return;
    }

    for (int Index = 0; Index < Result->Count; ++Index) {
        const SelectionRecord *CurrentRecord = &Result->Records[Index];
        printf("- %s | %s | %s | %d\n", CurrentRecord->student_id, CurrentRecord->student_name, CurrentRecord->course_name, CurrentRecord->score);
    }
}

/**
 * @brief 将查询结果导出为 CSV 文件。
 * @details 以标准 CSV 格式写入文件，便于后续在 Excel 或其他工具中查看和分析。
 * @param result 待导出的查询结果对象。
 * @param path 输出文件路径。
 * @return 导出成功返回 true，否则返回 false。
 */
bool SaveQueryResultToCsv(const QueryResult *Result, const char *Path) {
    if (Result == NULL || Path == NULL) {
        return false;
    }

    FILE *FilePointer = fopen(Path, "w");
    if (FilePointer == NULL) {
        return false;
    }

    fprintf(FilePointer, "学号,姓名,学院,课程编号,课程名称,学分,选课学期,选课日期,成绩\n");
    for (int Index = 0; Index < Result->Count; ++Index) {
        const SelectionRecord *CurrentRecord = &Result->Records[Index];
        fprintf(FilePointer, "%s,%s,%s,%s,%s,%.1f,%s,%s,%d\n",
                CurrentRecord->student_id,
                CurrentRecord->student_name,
                CurrentRecord->college,
                CurrentRecord->course_id,
                CurrentRecord->course_name,
                CurrentRecord->credit,
                CurrentRecord->semester,
                CurrentRecord->selected_date,
                CurrentRecord->score);
    }

    fclose(FilePointer);
    return true;
}
