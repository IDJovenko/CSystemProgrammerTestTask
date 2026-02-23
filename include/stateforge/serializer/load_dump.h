#ifndef LOAD_DUMP_H
#define LOAD_DUMP_H

#include "../../stat_data.h"
#include <sys/types.h>

ssize_t LoadData(StatData* data, const char* filename);

#endif //LOAD_DUMP_H