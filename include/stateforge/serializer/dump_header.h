#ifndef DUMP_HEADER_H
#define DUMP_HEADER_H

#include "../../stat_data.h"
#include <stddef.h>

// Для выравнивания при чтении/записи с помощью union
// устанавливаем размер равный размеру struct StatData
// рабоатет, если sizeof(StatData) < sizeof(headerContent)
typedef union DumpHeader {
    struct StatData offsetProvider;
    struct {
        // int32_t version;
        size_t array_size;
    } headerContent;
} DumpHeader;

#endif //DUMP_HEADER
