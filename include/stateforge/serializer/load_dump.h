#ifndef LOAD_DUMP_H
#define LOAD_DUMP_H

#include <sys/types.h>
#include "../../stat_data.h"

/// @brief Loads the StatData array from a file.
/// @param[out] data Pointer to the array of StatData.
/// @param[in] filename Name of the file to load the data from.
/// @return Number of elements read on success, -1 on failure.
ssize_t LoadDump(StatData** data, const char* filename);

#endif //LOAD_DUMP_H