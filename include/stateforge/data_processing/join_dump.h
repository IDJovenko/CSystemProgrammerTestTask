#ifndef JOIN_DUMP_H
#define JOIN_DUMP_H

#include "stat_data.h"
#include <stddef.h>
#include <sys/types.h>

/// @brief Joins two arrays of StatData into a single array.
/// @param[in] data1 First array of StatData. Will be modified (sorted) during processing.
/// @param[in] size1 Size of the first array.
/// @param[in] data2 Second array of StatData. Will be modified (sorted) during processing.
/// @param[in] size2 Size of the second array.
/// @param[out] result Pointer to the resulting array of StatData.
/// @return The size of the resulting array on success, -1 on failure.
ssize_t JoinDump(StatData* data1, size_t size1, StatData* data2, size_t size2, StatData** result);

#endif //JOIN_DUMP_H