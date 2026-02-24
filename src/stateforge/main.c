#include "stat_data.h"
#include "stateforge/data_processing/join_dump.h"
#include "stateforge/data_processing/sort_dump.h"
#include "serializer/load_dump.h"
#include "serializer/store_dump.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int fprint_statdata(FILE* stream, const StatData* data) {
    if (stream == NULL || data == NULL) {
        errno = EINVAL;
        perror("fprint_statdata");
        return -1;
    }
    // Получаем значение primary
    char primary_char = data->primary ? 'y' : 'n';
    
    // Формируем бинарное представление mode (3 бита)
    char mode_binary[4] = "000";  // 3 бита + null-терминатор
    unsigned int mode_val = data->mode & 0x07;  // гарантируем только 3 бита
    for (int i = 2; i >= 0; i--) {
        mode_binary[i] = (mode_val & 1) ? '1' : '0';
        mode_val >>= 1;
    }
    
    return fprintf(stream, "0x%016lx  %10d  %8.3e  %c  %3s\n",
                         data->id,           // id: 0x + 16 шестнадцатеричных цифр
                         data->count,        // count: 10 позиций, выравнивание вправо
                         data->cost,         // cost: научный формат с 3 знаками
                         primary_char,       // primary: 'y' или 'n'
                         mode_binary);       // mode: бинарный 
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <input1> <input2> <output>\n", argv[0]);
        return 1;
    }

    const char *input_path_1 = argv[1];
    const char *input_path_2 = argv[2];
    const char *output_path = argv[3];

    StatData *data1 = NULL;
    StatData *data2 = NULL;
    StatData *joined = NULL;

    ssize_t size1 = LoadDump(&data1, input_path_1);
    if (size1 < 0) {
        free(data1);
        return 1;
    }

    ssize_t size2 = LoadDump(&data2, input_path_2);
    if (size2 < 0) {
        free(data1);
        free(data2);
        return 1;
    }

    ssize_t joined_size = JoinDump(data1, (size_t)size1, data2, (size_t)size2, &joined);
    if (joined_size < 0) {
        free(data1);
        free(data2);
        free(joined);
        return 1;
    }

    if (joined_size > 0) {
        SortDump(joined, (size_t)joined_size);
    }

    size_t to_print = (joined_size < 10) ? (size_t)joined_size : 10U;
    for (size_t i = 0; i < to_print; ++i) {
        fprint_statdata(stdout, &joined[i]);
    }

    StatData empty_stub = {.id = 0, .count = 0, .cost = 0.0f, .primary = 0, .mode = 0};
    const StatData *to_store = (joined_size == 0) ? &empty_stub : joined;

    if (StoreDump(to_store, (size_t)joined_size, output_path) < 0) {
        free(data1);
        free(data2);
        free(joined);
        return 1;
    }

    free(data1);
    free(data2);
    free(joined);
    return 0;
}