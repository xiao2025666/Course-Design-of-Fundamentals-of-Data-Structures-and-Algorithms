#include "performance_test.h"

#include <stdio.h>
#include <time.h>

/**
 * @brief 简单演示性能测试流程。
 * @details 通过记录起止时钟计数，展示程序中基本的耗时测量方式。
 */
void RunPerformanceDemo(void) {
    clock_t StartTime = clock();
    for (int Index = 0; Index < 1000; ++Index) {
        (void)Index;
    }
    clock_t EndTime = clock();
    printf("性能测试演示：耗时 %ld ticks\n", (long)(EndTime - StartTime));
}
