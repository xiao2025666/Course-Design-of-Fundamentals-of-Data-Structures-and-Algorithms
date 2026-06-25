#include "performance_test.h"

#include <stdio.h>
#include <time.h>

void RunPerformanceDemo(void) {
    clock_t start = clock();
    for (int i = 0; i < 1000; ++i) {
        (void)i;
    }
    clock_t end = clock();
    printf("性能测试演示：耗时 %ld ticks\n", (long)(end - start));
}
