#ifndef STATISTICS_H
#define STATISTICS_H

#include "record_system.h"

void StatCourseCount(const RecordDatabase *db);
void StatStudentCredits(const RecordDatabase *db);
void StatCollegeDistribution(const RecordDatabase *db);
void StatSemesterHotness(const RecordDatabase *db);
void StatScoreSegments(const RecordDatabase *db);

#endif
