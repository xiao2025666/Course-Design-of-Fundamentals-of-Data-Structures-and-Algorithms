#ifndef QUERY_PROCESSOR_H
#define QUERY_PROCESSOR_H

#include <stdbool.h>
#include "record_system.h"

typedef struct {
    SelectionRecord *records;
    int count;
    int capacity;
} QueryResult;

void QueryResultInit(QueryResult *result);
void QueryResultFree(QueryResult *result);
bool BuildQueryResult(const RecordDatabase *db,
                      const char *course_name,
                      const char *semester,
                      const char *college,
                      int min_score,
                      int max_score,
                      QueryResult *result);
void SortQueryResult(QueryResult *result);
void PrintQueryResult(const QueryResult *result);
bool SaveQueryResultToCsv(const QueryResult *result, const char *path);

#endif
