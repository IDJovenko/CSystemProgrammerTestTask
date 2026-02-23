#include "../../../include/stateforge/data_processing/join_dump.h"
#include "../../../include/stateforge/data_processing/sort_dump.h"
#include "../../../include/contracts.h"
#include <string.h>
#include <errno.h>

static void MergePair(StatData* target, const StatData* source)
{
    target->count += source->count;
    target->cost  += source->cost;
    if (source->primary == 0)
        target->primary = 0;
    if (source->mode > target->mode)
        target->mode = source->mode;
}

ssize_t JoinDump(StatData *data1, size_t size1, StatData *data2, size_t size2, StatData **result)
{
    if (result == NULL || (size1 > 0 && data1 == NULL) || (size2 > 0 && data2 == NULL)) {
        errno = EINVAL;
        perror("JoinDump: invalid arguments");
        return -1;
    }

    *result = NULL;

    if (size1 + size2 > MAX_STATDATA_ARRAY_SIZE) {
        errno = ERANGE;
        perror("JoinDump: array size exceeds maximum");
        return -1;
    }

    size_t mergedSize = size1 + size2;
    if (mergedSize == 0)
        return 0;

    SortDumpWithComparator(data1, size1, compareByID);
    SortDumpWithComparator(data2, size2, compareByID);

    *result = malloc(mergedSize * sizeof(StatData));
    if (*result == NULL) {
        perror("JoinDump: memory allocation failed");
        return -1;
    }

    // Слияние отсортированных массивов с агрегацией по id
    size_t i = 0; // индекс для data1
    size_t j = 0; // индекс для data2
    size_t k = 0; // индекс для result
    while (i < size1 || j < size2) {
        StatData current;

        if (j >= size2 || (i < size1 && data1[i].id < data2[j].id)) {
            current = data1[i++];
        } else if (i >= size1 || data2[j].id < data1[i].id) {
            current = data2[j++];
        } else {
            /* id равны */
            current = data1[i++];
            MergePair(&current, &data2[j]);
            j++;
        }

        /* агрегируем одинаковые id из data1 */
        while (i < size1 && data1[i].id == current.id) {
            MergePair(&current, &data1[i]);
            i++;
        }

        /* агрегируем одинаковые id из data2 */
        while (j < size2 && data2[j].id == current.id) {
            MergePair(&current, &data2[j]);
            j++;
        }

        (*result)[k++] = current;
    }

    // Уменьшаем выделенную память, если итоговый размер меньше объединенного
    if (k < mergedSize) {
        StatData* shrink = realloc(*result, k * sizeof(StatData));
        if (shrink) {
            *result = shrink;
        } else {
            perror("JoinDump: memory reallocation failed");
            // В случае неудачи оставляем исходный указатель, так как он все еще валиден
        }
    }

    return (ssize_t)k;
}
