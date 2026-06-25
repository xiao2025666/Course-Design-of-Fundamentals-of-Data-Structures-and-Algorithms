#ifndef QUERY_PROCESSOR_H
#define QUERY_PROCESSOR_H

#include <stdbool.h>
#include "record_system.h"

typedef struct {
    SelectionRecord *Records;
    int Count;
    int Capacity;
} QueryResult;

void QueryResultInit(QueryResult *Result);
void QueryResultFree(QueryResult *Result);
bool BuildQueryResult(const RecordDatabase *Db,
                      const char *CourseName,
                      const char *Semester,
                      const char *College,
                      int MinScore,
                      int MaxScore,
                      QueryResult *Result);
void SortQueryResult(QueryResult *Result);
void PrintQueryResult(const QueryResult *Result);
bool SaveQueryResultToCsv(const QueryResult *Result, const char *Path);

#endif
