#include "file_io.h"

bool load_from_csv(RecordDatabase *db, const char *path) {
    return LoadRecordsFromCsv(db, path);
}

bool save_to_csv(const RecordDatabase *db, const char *path) {
    return SaveRecordsToCsv(db, path);
}
