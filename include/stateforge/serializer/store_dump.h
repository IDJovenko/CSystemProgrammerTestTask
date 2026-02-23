#ifndef LOAD_DUMP_H
#define LOAD_DUMP_H

#include "../../stat_data.h"
#include <sys/types.h>
#include <stddef.h>

ssize_t StoreDump(const StatData *data, size_t size, const char *filename);

#endif //LOAD_DUMP_H