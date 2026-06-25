#ifndef STORAGE_ADAPTER_H
#define STORAGE_ADAPTER_H

#include <stdbool.h>
#include "data_structure.h"

typedef struct StorageAdapter {
    bool (*insert)(void *storage, const CourseRecord *record);
    bool (*remove)(void *storage, const char *student_id, const char *course_id);
    CourseRecord *(*find)(void *storage, const char *student_id, const char *course_id);
    void (*traverse)(void *storage, void (*visit)(const CourseRecord *));
} StorageAdapter;

#endif
