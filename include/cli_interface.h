/**
 * @file cli_interface.h
 * @brief 命令行交互模块 - 头文件
 * @author Xiao Zexi (xiaonobadthing@qq.com)
 * @college Changsha University
 * @major Cybersecurity
 * @class Network02
 * @studentId B20250308228
 * @version 1.0.0
 * @date 2026-06-25
 *
 * @details
 * 该模块负责定义命令行菜单和交互控制接口，便于与记录管理模块解耦。
 */

#ifndef CLI_INTERFACE_H
#define CLI_INTERFACE_H

#include "record_system.h"

/**
 * @brief 输出命令行菜单。
 */
void PrintMenu(void);

/**
 * @brief 运行命令行交互循环。
 * @param db 数据库对象
 * @param default_path 默认数据文件路径
 * @return 退出时返回 0
 */
int RunCli(RecordDatabase *db, const char *default_path);

#endif // CLI_INTERFACE_H
