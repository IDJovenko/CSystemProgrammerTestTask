#include "sort_dump.h"
#include <stdlib.h>
#include <errno.h>

void SortDump(StatData *data, size_t size)
{
    SortDumpWithComparator(data, size, compareByCost);
}

void SortDumpWithComparator(StatData *data, size_t size, int (*cmp)(const void *, const void *))
{
    if (data == NULL || size == 0) {
        errno = EINVAL;
        perror("SortDump: invalid arguments");
        return;
    }

    qsort(data, size, sizeof(StatData), cmp);
}

int compareByCost(const void *a, const void *b) {
    const StatData *statA = (const StatData *)a;
    const StatData *statB = (const StatData *)b;

    if (statA->cost < statB->cost) {
        return -1;
    } else if (statA->cost > statB->cost) {
        return 1;
    } else {
        return 0;
    }
}

int compareByID(const void *a, const void *b) {
    const StatData *statA = (const StatData *)a;
    const StatData *statB = (const StatData *)b;

    if (statA->id < statB->id) {
        return -1;
    } else if (statA->id > statB->id) {
        return 1;
    } else {
        return 0;
    }
}
