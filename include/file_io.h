#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdbool.h>
#include "record_system.h"

bool load_from_csv(RecordDatabase *db, const char *path);
bool save_to_csv(const RecordDatabase *db, const char *path);

#endif
