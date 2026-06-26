/**
 * @file data_generator.c
 * @brief  测试数据生成器 - 该文件包含用于生成测试数据的函数和宏定义。
 * @author Xiao Zexi (xiaonobadthing@qq.com)
 * @college Changsha University
 * @major Cybersecurity
 * @class Network02
 * @studentId B20250308228
 * @version 1.0.0
 * @date 2026-06-23
 * 
 * 用法: ./data_generator [记录条数] [输出文data_generator.c件名]
 * 示例: ./data_generator 100 data/records.csv
 *       ./data_generator 10000 data/test_10k.csv
 * 
 * 如果未指定参数，默认生成100条保存为 data/records.csv
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <locale.h>

#define MAX_LINE_LEN 512//定义最大行数,防止缓冲区溢出导致安全问题


/*学生信息和课程信息结构体定义*/

/**
 * @brief 定义了一个学生信息结构体。
 * @note  存储学号,姓名，学院
 */
typedef struct {
    char studentId[13];      /**< 学号 */
    char name[11];           /**< 姓名 */
    char college[50];        /**< 学院 */
} StudentInfo;

/** @brief 课程信息结构体 
 * @note 该结构体用于存储课程编号、课程名称和学分信息
 */
typedef struct {
    const char *id;
    const char *name;
    float credit;
} CourseInfo;


/*--------常量和预设数据池--------*/

/**
 * @brief 创建了数组,规定了课程代码和课程名称以及学分的对应关系
 * @note 该数组用于生成学生选课记录时随机选择课程信息
 * @param COURSES 课程信息数组
 */
/**
 * @brief 课程信息池：随机生成学生选课记录时从中选择课程信息
 * @note 包含计算机、电子、数学、外语、经济等多个课程选项
 */
const CourseInfo COURSES[] = {
    {"CS300101", "程序设计基础", 3.0},
    {"CS300102", "数据结构与算法", 3.5},
    {"CS300103", "操作系统原理", 3.0},
    {"CS300104", "计算机网络", 3.0},
    {"CS300105", "数据库系统概论", 2.5},
    {"CS300106", "软件工程导论", 2.0},
    {"EE200101", "电路分析基础", 3.0},
    {"EE200102", "信号与系统", 3.5},
    {"MA100101", "高等数学A", 5.0},
    {"MA100102", "线性代数", 2.5},
    {"MA100103", "概率论与数理统计", 3.0},
    {"FL400101", "大学英语", 4.0},
    {"FL400102", "英语口语", 2.0},
    {"EC500101", "经济学原理", 3.0},
    {"EC500102", "管理学原理", 2.5}
};
#define COURSE_COUNT (sizeof(COURSES) / sizeof(COURSES[0]))



/**
 * @brief 定义测试数据生成时使用的字符常量池
 * @note 包含中文姓氏、名字和学院列表，用于随机生成学生姓名和学院信息
 */
// 中文姓氏（用于随机生成姓名）
const char *SURNAMES[] = {"张", "王", "李", "刘", "陈", "杨", "黄", "赵", "周", "吴", 
                          "徐", "孙", "马", "朱", "胡", "郭", "林", "何", "高", "郑"};
#define SURNAME_COUNT (sizeof(SURNAMES) / sizeof(SURNAMES[0]))

// 中文名字（第二字和第三字）
const char *GIVEN_NAMES[] = {"伟", "芳", "娜", "秀英", "敏", "静", "丽", "强", "磊", "洋", 
                             "勇", "艳", "杰", "娟", "涛", "明", "超", "秀兰", "霞", "平"};
#define GIVEN_COUNT (sizeof(GIVEN_NAMES) / sizeof(GIVEN_NAMES[0]))

// 学院列表（符合真实高校命名）
const char *COLLEGES[] = {"计算机科学与工程学院", "软件学院", "电子与信息工程学院", 
                          "数学与统计学院", "外国语学院", "经济管理学院", "人文社会科学学院"};
#define COLLEGE_COUNT (sizeof(COLLEGES) / sizeof(COLLEGES[0]))


/* ---------- 工具函数 ---------- */

/** @brief 生成随机学生信息
 * @param student 指向 StudentInfo 结构体的指针
 * @note 该函数会随机生成学号、姓名和学院，并填充到传入的 StudentInfo 结构体中
 */
void generate_random_student(StudentInfo *student) {
    // 生成随机学号
    snprintf(student->studentId, sizeof(student->studentId), "2023%08d", rand() % 100000000);

    // 生成随机姓名
    const char *surname = SURNAMES[rand() % SURNAME_COUNT];
    const char *given_name = GIVEN_NAMES[rand() % GIVEN_COUNT];
    snprintf(student->name, sizeof(student->name), "%s%s", surname, given_name);

    // 生成随机学院
    student->college[0] = '\0';
    strncat(student->college, COLLEGES[rand() % COLLEGE_COUNT], sizeof(student->college) - 1);
}

/** @brief 生成随机课程信息
 * @param course 指向 CourseInfo 结构体的指针
 * @note 从预定义课程池中随机选择一门课程
 */
void generate_random_course(CourseInfo *course) {
    *course = COURSES[rand() % COURSE_COUNT];
}


/**
 * @brief 生成一个随机的选课日期 (2022-01-01 ~ 2026-06-30)
 * @param buffer 缓冲区
 * @param size 缓冲区大小
 * @note 重点：保证部分记录在2023-09-01之前（测试过期删除）
 */
static void generate_date(char *buffer, size_t size) {
    int year, month, day;
    // 40%概率生成2022年或2023年初的日期（确保有大量过期数据）
    if (rand() % 10 < 4) {
        year = 2022 + rand() % 2;      // 2022 ~ 2023
        month = rand() % 12 + 1;
        day = rand() % 28 + 1;         // 保险起见，统一1-28日
    } else {
        year = 2023 + rand() % 3;      // 2023 ~ 2025
        month = rand() % 12 + 1;
        day = rand() % 28 + 1;
    }
    // 避免生成超出2026年6月的数据（课程设计规范）
    if (year == 2026 && month > 6) {
        month = 6;
        day = 15;
    }
    snprintf(buffer, size, "%04d-%02d-%02d", year, month, day);
}

/**
 * @brief 根据日期生成对应的学期字段 (YYYY-01 春季 / YYYY-02 秋季)
 * @param buffer 缓冲区
 * @param size 缓冲区大小
 * @param date 日期字符串
 */
static void generate_semester(char *buffer, size_t size, const char *date) {
    int year, month;
    sscanf(date, "%d-%d", &year, &month);
    // 3月-8月算春季(01)，9月-2月算秋季(02)（简单逻辑）
    if (month >= 3 && month <= 8) {
        snprintf(buffer, size, "%04d-01", year);
    } else {
        snprintf(buffer, size, "%04d-02", year);
    }
}

/**
 * @brief 生成一个随机的成绩 (0-100)，分布略偏向中等偏上
 * @note 80%概率生成 60-99 分，20%概率生成 0-59（补考/不及格）
 */
static int generate_score(void) {
    int score;
    // 80%概率生成 60-99 分，20%概率生成 0-59（补考/不及格）
    if (rand() % 10 < 8) {
        score = 60 + rand() % 40;  // 60 ~ 99
    } else {
        score = rand() % 60;       // 0 ~ 59
    }
    return score;
}

/**
 * @brief 检查字符串是否只包含数字字符
 * @param s 要检查的字符串
 * @return 如果字符串非空且每个字符都是 ASCII 数字字符 ('0'..'9')，返回 1；否则返回 0
 * @note 该函数用于判断命令行参数第一个值是否为记录数量，而不是文件路径。
 *       它不会认为空字符串、带符号、含空格或含小数点的字符串是数字。
 */
static int is_numeric_string(const char *s) {
    if (!s || !*s) {
        return 0; // 空指针或空字符串都不是有效数字字符串
    }
    while (*s) {
        if (*s < '0' || *s > '9') {
            return 0; // 遇到非数字字符立即返回 false
        }
        s++;
    }
    return 1; // 全部字符都是数字
}

/**
 * @brief 创建输出文件路径中指定的目录
 * @param path 输出文件完整路径，例如 "data/records.csv"
 * @note 该函数只创建路径中的目录部分，不创建文件本身。
 *       如果 path 不包含路径分隔符，则认为文件在当前目录，无需创建目录。
 *       在 Windows 下使用 _mkdir，在类 Unix 系统下使用 mkdir。
 */
static void ensure_output_directory(const char *path) {
    char dir[512];
    const char *last_sep = NULL;

    /* 找到路径中最后一个路径分隔符，支持 '/' 和 '\' */
    for (const char *p = path; *p; ++p) {
        if (*p == '/' || *p == '\\') {
            last_sep = p;
        }
    }

    /* 如果没有路径分隔符，则文件路径没有目录部分，直接返回 */
    if (!last_sep) {
        return;
    }

    /* 计算目录部分长度，例如从 "data/records.csv" 提取 "data" */
    size_t len = (size_t)(last_sep - path);
    if (len == 0 || len >= sizeof(dir)) {
        return; // 目录名长度不合法或超出缓冲区大小
    }

    memcpy(dir, path, len);
    dir[len] = '\0'; // 以空字符结束目录字符串
}

static void InitConsoleEncoding(void) {
#ifdef _WIN32
    SetConsoleCP(936);
    SetConsoleOutputCP(936);
#endif
    if (setlocale(LC_ALL, ".936") == NULL) {
        setlocale(LC_ALL, "");
    }
}

/* ---------- 主函数 ---------- */
/**
 * @brief 程序入口，生成指定数量的选课测试数据并保存为CSV文件
 * @param argc 命令行参数个数
 * @param argv 命令行参数值数组
 * @return 返回0表示成功，非0表示失败
 */
int main(int argc, char *argv[]) {
    InitConsoleEncoding();

    int record_count = 100;                   // 默认生成记录数量
    const char *output_file = "data/records.csv"; // 默认输出文件路径

    // 解析命令行参数：第一参数为记录数量或输出文件路径，第二参数为输出文件路径
    if (argc >= 2) {
        if (is_numeric_string(argv[1])) {
            record_count = atoi(argv[1]);
            if (record_count <= 0) {
                record_count = 100;
            }
        } else {
            output_file = argv[1];
        }
    }
    if (argc >= 3) {
        record_count = atoi(argv[1]);
        if (record_count <= 0) {
            record_count = 100;
        }
        output_file = argv[2];
    }

    // 使用当前时间初始化随机数种子，确保每次运行生成不同数据
    srand((unsigned int)time(NULL));

    ensure_output_directory(output_file);
    FILE *fp = fopen(output_file, "w");
    if (!fp) {
        fprintf(stderr, "错误: 无法创建文件 %s\n", output_file);
        fprintf(stderr, "提示: 请确保 data 目录已存在（可用 mkdir data 创建）\n");
        return 1;
    }

    // 写入CSV表头，字段顺序与生成记录保持一致
    fprintf(fp, "学号,姓名,学院,课程编号,课程名称,学分,选课学期,选课日期,成绩\n");

    for (int i = 0; i < record_count; i++) {
        StudentInfo student;
        CourseInfo course;
        char semester[8];
        char date[11];
        int score;

        // 依次生成学生信息、课程信息、选课日期、学期和成绩
        generate_random_student(&student);
        generate_random_course(&course);
        generate_date(date, sizeof(date));
        generate_semester(semester, sizeof(semester), date);
        score = generate_score();

        // 将这条记录写入CSV文件
        fprintf(fp, "%s,%s,%s,%s,%s,%.1f,%s,%s,%d\n",
                student.studentId, student.name, student.college,
                course.id, course.name, course.credit,
                semester, date, score);
    }

    fclose(fp);
    printf("成功生成 %d 条选课记录，已保存至: %s\n", record_count, output_file);
    printf("提示: 可用Excel打开此文件查看数据，或直接用文本编辑器查看\n");

    return 0;
}

