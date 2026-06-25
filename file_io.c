#include "file_io.h"

bool load_from_csv(RecordDatabase *Database, const char *Path) {
    return LoadRecordsFromCsv(Database, Path);
}

bool save_to_csv(const RecordDatabase *Database, const char *Path) {
    return SaveRecordsToCsv(Database, Path);
}
