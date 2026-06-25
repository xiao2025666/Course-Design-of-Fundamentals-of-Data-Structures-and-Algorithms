#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

#include "cli_interface.h"
#include "file_io.h"
#include "record_system.h"

/**
 * @file main.c
 * @brief 程序入口文件
 * @author Xiao Zexi (xiaonobadthing@qq.com)
 * @college Changsha University
 * @major Cybersecurity
 * @class Network02
 * @studentId B20250308228
 * @version 1.0.0
 * @date 2026-06-25
 *
 * @details
 * 该文件负责初始化系统，并将控制权交给命令行交互模块与记录管理模块。
 */
int main(int argc, char *argv[]) {
    const char *data_path = (argc >= 2) ? argv[1] : "data/records.csv";

    setlocale(LC_ALL, "");

    RecordDatabase db;
    RecordDatabaseInit(&db);

    if (load_from_csv(&db, data_path)) {
        printf("已加载 %d 条选课记录，文件：%s\n", db.count, data_path);
    } else {
        printf("未找到数据文件 %s，程序将从空数据库启动。\n", data_path);
    }

    RunCli(&db, data_path);

    save_to_csv(&db, data_path);
    RecordDatabaseFree(&db);
    return 0;
}