#include "performance_test.h"

#include "hash_table.h"
#include "record_system.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

static double ElapsedMs(clock_t start, clock_t end) {
    return ((double)(end - start) * 1000.0) / (double)CLOCKS_PER_SEC;
}

static void FillRecord(CourseRecord *record, int index) {
    memset(record, 0, sizeof(*record));
    snprintf(record->student_id, sizeof(record->student_id), "S%05d", index);
    snprintf(record->student_name, sizeof(record->student_name), "Student%d", index);
    snprintf(record->college, sizeof(record->college), "College%d", index % 10);
    snprintf(record->course_id, sizeof(record->course_id), "C%05d", index);
    snprintf(record->course_name, sizeof(record->course_name), "Course%d", index);
    record->credit = (float)((index % 5) + 1);
    snprintf(record->semester, sizeof(record->semester), "2024-2");
    snprintf(record->selected_date, sizeof(record->selected_date), "2024-09-01");
    record->score = index % 101;
}

static void RunDynamicArrayBenchmark(int count) {
    RecordDatabase db;
    RecordDatabaseInit(&db);

    clock_t start = clock();
    for (int i = 0; i < count; ++i) {
        CourseRecord record;
        FillRecord(&record, i);
        InsertRecord(&db, &record);
    }
    clock_t insert_end = clock();

    for (int i = 0; i < count; ++i) {
        char student_id[16];
        char course_id[16];
        snprintf(student_id, sizeof(student_id), "S%05d", i);
        snprintf(course_id, sizeof(course_id), "C%05d", i);
        (void)FindRecordByKey(&db, student_id, course_id);
    }
    clock_t find_end = clock();

    printf("动态数组：插入 %d 条记录耗时 %.3f ms\n", count, ElapsedMs(start, insert_end));
    printf("动态数组：查找 %d 条记录耗时 %.3f ms\n", count, ElapsedMs(insert_end, find_end));

    RecordDatabaseFree(&db);
}

static void RunHashTableBenchmark(int count) {
    HashTable table;
    HashTableInit(&table);

    clock_t start = clock();
    for (int i = 0; i < count; ++i) {
        CourseRecord record;
        FillRecord(&record, i);
        HashTableInsert(&table, &record);
    }
    clock_t insert_end = clock();

    for (int i = 0; i < count; ++i) {
        char student_id[16];
        char course_id[16];
        snprintf(student_id, sizeof(student_id), "S%05d", i);
        snprintf(course_id, sizeof(course_id), "C%05d", i);
        (void)HashTableFind(&table, student_id, course_id);
    }
    clock_t find_end = clock();

    printf("哈希表：插入 %d 条记录耗时 %.3f ms\n", count, ElapsedMs(start, insert_end));
    printf("哈希表：查找 %d 条记录耗时 %.3f ms\n", count, ElapsedMs(insert_end, find_end));

    HashTableDestroy(&table);
}

/**
 * @brief 运行动态数组与哈希表的插入查找性能测试。
 */
void RunPerformanceDemo(void) {
    const int count = 20000;
    printf("性能测试开始（测试数据量：%d 条）...\n", count);
    RunDynamicArrayBenchmark(count);
    RunHashTableBenchmark(count);
}
