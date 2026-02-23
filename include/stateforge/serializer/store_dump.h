#ifndef STORE_DUMP_H
#define STORE_DUMP_H

#include "../../stat_data.h"
#include <sys/types.h>
#include <stddef.h>

/// @brief Stores the given StatData array into a file.
/// @param data Pointer to the array of StatData.
/// @param size Number of elements in the array.
/// @param filename Name of the file to store the data.
/// @return Number of elements written on success, -1 on failure.
ssize_t StoreDump(const StatData *data, size_t size, const char *filename);

#endif //STORE_DUMP_H