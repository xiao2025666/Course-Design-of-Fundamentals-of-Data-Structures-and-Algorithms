#ifndef BATCH_DELETE_H
#define BATCH_DELETE_H

#include "record_system.h"

int DeleteExpiredByDate(RecordDatabase *db, const char *base_date);

#endif
