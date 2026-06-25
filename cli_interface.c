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

int RunCli(RecordDatabase *db, const char *default_path) {
    int choice = 0;

    while (choice != 9) {
        PrintMenu();
        if (scanf("%d", &choice) != 1) {
            ClearInputBuffer();
            choice = 0;
        }

        switch (choice) {
            case 1:
                PrintAllRecords(db);
                break;
            case 2: {
                SelectionRecord record;
                memset(&record, 0, sizeof(record));
                printf("请输入学号：");
                scanf("%15s", record.student_id);
                printf("请输入姓名：");
                scanf("%63s", record.student_name);
                printf("请输入学院：");
                scanf("%63s", record.college);
                printf("请输入课程编号：");
                scanf("%15s", record.course_id);
                printf("请输入课程名称：");
                scanf("%63s", record.course_name);
                printf("请输入学分：");
                scanf("%f", &record.credit);
                printf("请输入学期：");
                scanf("%7s", record.semester);
                printf("请输入选课日期：");
                scanf("%11s", record.selected_date);
                printf("请输入成绩：");
                scanf("%d", &record.score);
                InsertRecord(db, &record);
                break;
            }
            case 3: {
                char student_id[MAX_ID_LEN];
                char course_id[MAX_ID_LEN];
                printf("请输入学号：");
                scanf("%15s", student_id);
                printf("请输入课程编号：");
                scanf("%15s", course_id);
                if (DeleteRecordByKey(db, student_id, course_id)) {
                    puts("删除成功。" );
                } else {
                    puts("删除失败。" );
                }
                break;
            }
            case 4: {
                char student_id[MAX_ID_LEN];
                char course_id[MAX_ID_LEN];
                int new_score = 0;
                printf("请输入学号：");
                scanf("%15s", student_id);
                printf("请输入课程编号：");
                scanf("%15s", course_id);
                printf("请输入新成绩：");
                scanf("%d", &new_score);
                if (UpdateRecordScore(db, student_id, course_id, new_score)) {
                    puts("更新成功。" );
                } else {
                    puts("更新失败。" );
                }
                break;
            }
            case 5: {
                char student_id[MAX_ID_LEN];
                char course_id[MAX_ID_LEN];
                printf("请输入学号：");
                scanf("%15s", student_id);
                printf("请输入课程编号：");
                scanf("%15s", course_id);
                SelectionRecord *record = FindRecordByKey(db, student_id, course_id);
                if (record != NULL) {
                    printf("找到记录：%s | %s | %d\n", record->student_name, record->course_name, record->score);
                } else {
                    puts("未找到记录。" );
                }
                break;
            }
            case 6: {
                char course_name[MAX_TEXT_LEN];
                char semester[MAX_SEMESTER_LEN];
                char college[MAX_TEXT_LEN];
                int min_score = 0;
                int max_score = 100;
                memset(course_name, 0, sizeof(course_name));
                memset(semester, 0, sizeof(semester));
                memset(college, 0, sizeof(college));
                printf("请输入课程名称（可留空）：");
                scanf("%63s", course_name);
                printf("请输入学期（可留空）：");
                scanf("%7s", semester);
                printf("请输入学院（可留空）：");
                scanf("%63s", college);
                printf("请输入最低成绩：");
                scanf("%d", &min_score);
                printf("请输入最高成绩：");
                scanf("%d", &max_score);
                FilterAndPrint(db, course_name, semester, college, min_score, max_score);
                break;
            }
            case 7:
                ShowStatistics(db);
                break;
            case 8: {
                char base_date[MAX_DATE_LEN];
                printf("请输入基准日期（YYYY-MM-DD）：");
                scanf("%11s", base_date);
                int removed = DeleteExpiredRecords(db, base_date);
                printf("已删除 %d 条过期记录。\n", removed);
                break;
            }
            case 9:
                if (default_path != NULL) {
                    SaveRecordsToCsv(db, default_path);
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
