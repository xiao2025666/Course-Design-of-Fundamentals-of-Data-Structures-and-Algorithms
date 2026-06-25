#include "data_structure.h"

#include <stdio.h>
#include <string.h>

void copy_record(CourseRecord *Destination, const CourseRecord *Source) {
    if (Destination == NULL || Source == NULL) {
        return;
    }
    *Destination = *Source;
}

void print_record(const CourseRecord *Record) {
    if (Record == NULL) {
        return;
    }

    printf("学号:%s | 姓名:%s | 课程:%s | 成绩:%d\n",
           Record->student_id,
           Record->student_name,
           Record->course_name,
           Record->score);
}

unsigned int hash_code(const char *Value) {
    unsigned int Seed = 131;
    unsigned int Hash = 0;

    if (Value == NULL) {
        return 0;
    }

    while (*Value) {
        Hash = Hash * Seed + (unsigned char)(*Value++);
    }

    return Hash;
}

bool is_record_expired(const CourseRecord *Record, const char *BaseDate) {
    if (Record == NULL || BaseDate == NULL) {
        return false;
    }

    return strcmp(Record->selected_date, BaseDate) < 0;
}
