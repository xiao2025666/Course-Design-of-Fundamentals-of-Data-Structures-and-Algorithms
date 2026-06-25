#include "batch_delete.h"

#include <stdio.h>
#include <string.h>

/**
 * @brief 删除早于指定日期的过期记录。
 * @details 先询问用户确认，再将不满足日期条件的记录移出数据库。
 */
int DeleteExpiredByDate(RecordDatabase *Database, const char *BaseDate) {
    if (Database == NULL || BaseDate == NULL) {
        return 0;
    }

    printf("即将删除早于 %s 的记录，确认删除？(y/n): ", BaseDate);
    char Answer = getchar();
    if (Answer != 'y' && Answer != 'Y') {
        return 0;
    }

    int RemovedCount = 0;
    int WriteIndex = 0;
    for (int Index = 0; Index < Database->count; ++Index) {
        if (strcmp(Database->records[Index].selected_date, BaseDate) < 0) {
            RemovedCount++;
        } else {
            Database->records[WriteIndex++] = Database->records[Index];
        }
    }
    Database->count = WriteIndex;
    return RemovedCount;
}
