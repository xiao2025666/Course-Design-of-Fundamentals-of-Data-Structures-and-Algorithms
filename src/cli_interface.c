/**
 * @file cli_interface.c
 * @brief 命令行交互模块 - 实现文件
 * @author Xiao Zexi (xiaonobadthing@qq.com)
 * @college Changsha University
 * @major Cybersecurity
 * @class Network02
 * @studentId B20250308228
 * @version 1.0.0
 * @date 2026-06-25
 *
 * @details
 * 该模块实现命令行菜单、输入处理和功能分发。
 */

#include "cli_interface.h"

static void ClearInputBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

void PrintMenu(void) {
    puts("===== 选课记录管理系统 =====");
    puts("1. 查看所有记录");
    puts("2. 插入记录");
    puts("3. 删除记录");
    puts("4. 修改成绩");
    puts("5. 查找记录");
    puts("6. 筛选并输出");
    puts("7. 统计分析");
    puts("8. 删除过期记录");
    puts("9. 保存并退出");
    puts("请输入选项：");
}

int RunCli(RecordDatabase *Database, const char *DefaultPath) {
    int Choice = 0;

    while (Choice != 9) {
        PrintMenu();
        if (scanf("%d", &Choice) != 1) {
            ClearInputBuffer();
            Choice = 0;
        }

        switch (Choice) {
            case 1:
                PrintAllRecords(Database);
                break;
            case 2: {
                SelectionRecord Record;
                memset(&Record, 0, sizeof(Record));
                printf("请输入学号：");
                scanf("%15s", Record.student_id);
                printf("请输入姓名：");
                scanf("%63s", Record.student_name);
                printf("请输入学院：");
                scanf("%63s", Record.college);
                printf("请输入课程编号：");
                scanf("%15s", Record.course_id);
                printf("请输入课程名称：");
                scanf("%63s", Record.course_name);
                printf("请输入学分：");
                scanf("%f", &Record.credit);
                printf("请输入学期：");
                scanf("%7s", Record.semester);
                printf("请输入选课日期：");
                scanf("%11s", Record.selected_date);
                printf("请输入成绩：");
                scanf("%d", &Record.score);
                InsertRecord(Database, &Record);
                break;
            }
            case 3: {
                char StudentId[MAX_ID_LEN];
                char CourseId[MAX_ID_LEN];
                printf("请输入学号：");
                scanf("%15s", StudentId);
                printf("请输入课程编号：");
                scanf("%15s", CourseId);
                if (DeleteRecordByKey(Database, StudentId, CourseId)) {
                    puts("删除成功。" );
                } else {
                    puts("删除失败。" );
                }
                break;
            }
            case 4: {
                char StudentId[MAX_ID_LEN];
                char CourseId[MAX_ID_LEN];
                int NewScore = 0;
                printf("请输入学号：");
                scanf("%15s", StudentId);
                printf("请输入课程编号：");
                scanf("%15s", CourseId);
                printf("请输入新成绩：");
                scanf("%d", &NewScore);
                if (UpdateRecordScore(Database, StudentId, CourseId, NewScore)) {
                    puts("更新成功。" );
                } else {
                    puts("更新失败。" );
                }
                break;
            }
            case 5: {
                char StudentId[MAX_ID_LEN];
                char CourseId[MAX_ID_LEN];
                printf("请输入学号：");
                scanf("%15s", StudentId);
                printf("请输入课程编号：");
                scanf("%15s", CourseId);
                SelectionRecord *Record = FindRecordByKey(Database, StudentId, CourseId);
                if (Record != NULL) {
                    printf("找到记录：%s | %s | %d\n", Record->student_name, Record->course_name, Record->score);
                } else {
                    puts("未找到记录。" );
                }
                break;
            }
            case 6: {
                char CourseName[MAX_TEXT_LEN];
                char Semester[MAX_SEMESTER_LEN];
                char College[MAX_TEXT_LEN];
                int MinScore = 0;
                int MaxScore = 100;
                memset(CourseName, 0, sizeof(CourseName));
                memset(Semester, 0, sizeof(Semester));
                memset(College, 0, sizeof(College));
                printf("请输入课程名称（可留空）：");
                scanf("%63s", CourseName);
                printf("请输入学期（可留空）：");
                scanf("%7s", Semester);
                printf("请输入学院（可留空）：");
                scanf("%63s", College);
                printf("请输入最低成绩：");
                scanf("%d", &MinScore);
                printf("请输入最高成绩：");
                scanf("%d", &MaxScore);
                FilterAndPrint(Database, CourseName, Semester, College, MinScore, MaxScore);
                break;
            }
            case 7:
                ShowStatistics(Database);
                break;
            case 8: {
                char BaseDate[MAX_DATE_LEN];
                printf("请输入基准日期（YYYY-MM-DD）：");
                scanf("%11s", BaseDate);
                int Removed = DeleteExpiredRecords(Database, BaseDate);
                printf("已删除 %d 条过期记录。\n", Removed);
                break;
            }
            case 9:
                if (DefaultPath != NULL) {
                    SaveRecordsToCsv(Database, DefaultPath);
                }
                puts("已保存并退出。\n");
                break;
            default:
                puts("无效选项。\n");
                break;
        }

        ClearInputBuffer();
    }

    return 0;
}
