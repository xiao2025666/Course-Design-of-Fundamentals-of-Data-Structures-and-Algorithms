/**
 * @file record_system.h
 * @brief 选课记录管理模块 - 头文件
 * @author Xiao Zexi (xiaonobadthing@qq.com)
 * @college Changsha University
 * @major Cybersecurity
 * @class Network02
 * @studentId B20250308228
 * @version 1.0.0
 * @date 2026-06-25
 *
 * @details
 * 该模块负责定义选课记录的数据结构与基础操作接口，包含
 * CSV 文件的加载、保存、插入、删除、查找、筛选、统计与过期记录清理。
 */

#ifndef RECORD_SYSTEM_H
#define RECORD_SYSTEM_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_structure.h"

/**
 * @brief 单条选课记录结构体。
 * @details 保存学生、课程以及成绩等字段信息。
 */
typedef CourseRecord SelectionRecord;

/**
 * @brief 记录数据库容器。
 * @details 使用动态数组存储多条选课记录。
 */
typedef struct {
    SelectionRecord *records;
    int count;
    int capacity;
} RecordDatabase;

/**
 * @brief 初始化数据库对象。
 * @param db 待初始化的数据库对象
 */
void RecordDatabaseInit(RecordDatabase *db);

/**
 * @brief 释放数据库对象占用的内存。
 * @param db 待释放的数据库对象
 */
void RecordDatabaseFree(RecordDatabase *db);

/**
 * @brief 从 CSV 文件加载选课记录。
 * @param db 目标数据库对象
 * @param path CSV 文件路径
 * @return 加载成功返回 true，否则返回 false
 */
bool LoadRecordsFromCsv(RecordDatabase *db, const char *path);

/**
 * @brief 将数据库中的记录保存为 CSV 文件。
 * @param db 数据库对象
 * @param path 输出文件路径
 * @return 保存成功返回 true，否则返回 false
 */
bool SaveRecordsToCsv(const RecordDatabase *db, const char *path);

/**
 * @brief 插入一条选课记录。
 * @param db 数据库对象
 * @param record 待插入的记录
 * @return 插入成功返回 true，否则返回 false
 */
bool InsertRecord(RecordDatabase *db, const SelectionRecord *record);

/**
 * @brief 根据学号和课程编号删除记录。
 * @param db 数据库对象
 * @param student_id 学号
 * @param course_id 课程编号
 * @return 删除成功返回 true，否则返回 false
 */
bool DeleteRecordByKey(RecordDatabase *db, const char *student_id, const char *course_id);

/**
 * @brief 修改指定记录的成绩。
 * @param db 数据库对象
 * @param student_id 学号
 * @param course_id 课程编号
 * @param new_score 新成绩
 * @return 更新成功返回 true，否则返回 false
 */
bool UpdateRecordScore(RecordDatabase *db, const char *student_id, const char *course_id, int new_score);

/**
 * @brief 按学号和课程编号查找记录。
 * @param db 数据库对象
 * @param student_id 学号
 * @param course_id 课程编号
 * @return 找到则返回记录指针，否则返回 NULL
 */
SelectionRecord *FindRecordByKey(RecordDatabase *db, const char *student_id, const char *course_id);

/**
 * @brief 打印当前数据库中的所有记录。
 * @param db 数据库对象
 */
void PrintAllRecords(const RecordDatabase *db);

/**
 * @brief 输出统计信息。
 * @param db 数据库对象
 */
void ShowStatistics(const RecordDatabase *db);

/**
 * @brief 按条件筛选记录并打印结果。
 * @param db 数据库对象
 * @param course_name 课程名称关键字
 * @param semester 学期
 * @param college 学院名称
 * @param min_score 最低成绩
 * @param max_score 最高成绩
 */
void FilterAndPrint(const RecordDatabase *db, const char *course_name, const char *semester, const char *college, int min_score, int max_score);

/**
 * @brief 删除早于给定日期的过期记录。
 * @param db 数据库对象
 * @param base_date 基准日期
 * @return 删除的记录数量
 */
int DeleteExpiredRecords(RecordDatabase *db, const char *base_date);

#endif // RECORD_SYSTEM_H
