#include "batch_delete.h"

#include <stdio.h>
#include <string.h>

int DeleteExpiredByDate(RecordDatabase *db, const char *base_date) {
    if (db == NULL || base_date == NULL) {
        return 0;
    }

    printf("即将删除早于 %s 的记录，确认删除？(y/n): ", base_date);
    char answer = getchar();
    if (answer != 'y' && answer != 'Y') {
        return 0;
    }

    int removed = 0;
    int write_index = 0;
    for (int i = 0; i < db->count; ++i) {
        if (strcmp(db->records[i].selected_date, base_date) < 0) {
            removed++;
        } else {
            db->records[write_index++] = db->records[i];
        }
    }
    db->count = write_index;
    return removed;
}
