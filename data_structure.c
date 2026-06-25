#include "data_structure.h"

#include <stdio.h>
#include <string.h>

void copy_record(CourseRecord *dst, const CourseRecord *src) {
    if (dst == NULL || src == NULL) {
        return;
    }
    *dst = *src;
}

void print_record(const CourseRecord *record) {
    if (record == NULL) {
        return;
    }

    printf("学号:%s | 姓名:%s | 课程:%s | 成绩:%d\n",
           record->student_id,
           record->student_name,
           record->course_name,
           record->score);
}

unsigned int hash_code(const char *value) {
    unsigned int seed = 131;
    unsigned int hash = 0;

    if (value == NULL) {
        return 0;
    }

    while (*value) {
        hash = hash * seed + (unsigned char)(*value++);
    }

    return hash;
}

bool is_record_expired(const CourseRecord *record, const char *base_date) {
    if (record == NULL || base_date == NULL) {
        return false;
    }

    return strcmp(record->selected_date, base_date) < 0;
}
