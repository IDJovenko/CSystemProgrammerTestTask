#ifndef SORT_DUMP_H
#define SORT_DUMP_H

#include "stat_data.h"
#include <stddef.h>

/// @brief Sorts the StatData array by the 'cost' field in ascending order
/// @param data Pointer to the array of StatData
/// @param size Number of elements in the array
void SortDump(StatData* data, size_t size);

/// @brief Sorts the StatData array using a custom comparator function
/// @param data Pointer to the array of StatData
/// @param size Number of elements in the array
/// @param cmp Comparison function to determine the order of elements
void SortDumpWithComparator(StatData* data, size_t size, int (*cmp)(const void*, const void*));

/// @brief Comparator function to sort StatData by 'cost' in ascending order
int compareByCost(const void *a, const void *b);
/// @brief Comparator function to sort StatData by 'id' in ascending order
int compareByID(const void *a, const void *b);


#endif //SORT_DUMP_H