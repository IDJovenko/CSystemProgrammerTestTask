#define _GNU_SOURCE
#include "src/stateforge/serializer/store_dump.c"
#include <stdlib.h>


void fill_data(StatData *data, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        data[i].id = i + 1L;
        data[i].count = 1;
        data[i].cost = (float)(i + 1);
        data[i].primary = 0;
        data[i].mode = (short)(i % 3 + 1); // Циклическое изменение режима от 1 до 3
    }
}

int main() {
    const size_t array_size = 10000;
    
    // Динамическое выделение памяти под массив
    StatData* statDataArray = malloc(array_size * sizeof(StatData));
    if (!statDataArray) {
        perror("Ошибка выделения памяти");
        return -1;
    }

    // Заполнение массива случайными данными
    fill_data(statDataArray, array_size);

    // Сохранение данных в файл
    StoreDump(statDataArray, array_size, "large_test_file_to_save.dump");

    free(statDataArray); // Освобождение выделенной памяти

    printf("Файл успешно сохранён.\n");
    return 0;
}
