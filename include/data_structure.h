/**
 * @file data_structure.h
 * @brief 对学生选课记录数据结构的定义 - 头文件
 * @author Xiao Zexi (xiaonobadthing@qq.com)
 * @college Changsha University
 * @major Cybersecurity
 * @class Network02
 * @studentId B20250308228
 * @version 1.0.0
 * @date 2026-06-25
 *
 * @details
 * 该头文件定义了用于存储学生选课信息的数据结构，包括课程信息、学生信息
 * 以及选课系统的整体管理结构。该模块可用于实现学生选课记录的增删改查。
 */

#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <stdbool.h>

#define MAX_TEXT_LEN 64
#define MAX_COURSE_NAME_LEN 64
#define MAX_STUDENT_NAME_LEN 32
#define MAX_ID_LEN 16
#define MAX_SEMESTER_LEN 8
#define MAX_DATE_LEN 12
#define MAX_COURSES_PER_STUDENT 10
#define MAX_STUDENTS 200

typedef struct {
    char student_id[MAX_ID_LEN];
    char student_name[MAX_TEXT_LEN];
    char college[MAX_TEXT_LEN];
    char course_id[MAX_ID_LEN];
    char course_name[MAX_COURSE_NAME_LEN];
    float credit;
    char semester[MAX_SEMESTER_LEN];
    char selected_date[MAX_DATE_LEN];
    int score;
} CourseRecord;

/**
 * @brief 课程信息结构体。
 * @details 记录课程编号、课程名称、授课教师、学分以及当前是否被选中。
 */
typedef struct {
    char course_id[MAX_ID_LEN];
    char course_name[MAX_COURSE_NAME_LEN];
    char teacher[MAX_STUDENT_NAME_LEN];
    int credit;
    bool selected;
} Course;

/**
 * @brief 学生选课记录结构体。
 * @details 保存单个学生的基本信息以及其选课课程列表。
 */
typedef struct {
    char student_id[MAX_ID_LEN];
    char student_name[MAX_STUDENT_NAME_LEN];
    char major[MAX_STUDENT_NAME_LEN];
    int grade;
    int course_count;
    Course courses[MAX_COURSES_PER_STUDENT];
} StudentRecord;

/**
 * @brief 选课系统总控结构体。
 * @details 维护系统中所有学生记录的数组和当前学生数量。
 */
typedef struct {
    StudentRecord students[MAX_STUDENTS];
    int student_count;
} CourseSelectionSystem;

void copy_record(CourseRecord *dst, const CourseRecord *src);
void print_record(const CourseRecord *record);
unsigned int hash_code(const char *value);
bool is_record_expired(const CourseRecord *record, const char *base_date);

#endif // DATA_STRUCTURE_H
