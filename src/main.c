#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "cli_interface.h"
#include "file_io.h"
#include "record_system.h"

static void InitConsoleEncoding(void) {
#ifdef _WIN32
    SetConsoleCP(936);
    SetConsoleOutputCP(936);
#endif
    if (setlocale(LC_ALL, ".936") == NULL) {
        setlocale(LC_ALL, "");
    }
}

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
    const char *DataPath = (argc >= 2) ? argv[1] : "data/records.csv";

    InitConsoleEncoding();

    RecordDatabase Database;
    RecordDatabaseInit(&Database);

    if (load_from_csv(&Database, DataPath)) {
        printf("已加载 %d 条选课记录，文件：%s\n", Database.count, DataPath);
    } else {
        printf("未找到数据文件 %s，程序将从空数据库启动。\n", DataPath);
    }

    RunCli(&Database, DataPath);

    save_to_csv(&Database, DataPath);
    RecordDatabaseFree(&Database);
    return 0;
}