#include "serializer/store_dump.h"
#include "serializer/dump_header.h"
#include "contracts.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>

ssize_t StoreDump(const StatData *data, size_t size, const char *filename)
{
    if ((data == NULL && size != 0)) {
        errno = EINVAL;
        perror("StoreDump: 'data' argument is NULL, but 'size' isn't");
        return -1;
    }
    if (filename == NULL) {
        errno = EINVAL;
        perror("StoreDump: 'filename' argument is NULL");
        return -1;
    }
    if (size > MAX_STATDATA_ARRAY_SIZE) {
        errno = ERANGE;
        perror("StoreDump: size exceeds MAX_STATDATA_ARRAY_SIZE");
        return -1;
    }
    
    // Получаем файловый дескриптор
    //  O_WRONLY - для записи; O_CREAT - создаём файл, если не существует;
    //  O_TRUNC - если файл существет, то урезаем длину до нуля
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("StoreDump: open failed");
        return -1;
    }

    
    // количесвто байт для записи в файл
    // эквивалентно sizeof(StatData)*(size+1) (см. DumpHeader)
    size_t sizeToWrite = sizeof(DumpHeader) + sizeof(StatData)*size;
    // Убеждаемся в коррекности приведения size_t к off_t
    static_assert(sizeof(off_t) == 8, "off_t must be 64-bit");
    
    // Выделяем дисковое пространство под данные
    // TODO: можно добавить обработку EINTR
    if (fallocate(fd, 0, 0, (off_t)sizeToWrite) == -1) {
        // Если функция не поддерживается ФС (EOPNOTSUPP),
        // то просто продолжаем
        // write ниже будет выделять - сам (большая фрагментация)
        if (errno == EOPNOTSUPP) {
            // продолжаем - это не ошибка для данной ФС
        }
        else {
            perror("StoreDump: fallocate failed");
            close(fd);
            return -1;
        }
    }

    // Подготавливаем заголовок
    DumpHeader header = {
        .headerContent = { .array_size = size }
    };
    // Записываем заголовок
    if (write(fd, &header, sizeof(DumpHeader)) == -1) {
        perror("StoreDump: header write failed");
        close(fd);
        return -1;
    }

    // Записываем данные
    // TODO: можно(/нужно) добавить обработку EINTR
    ssize_t written = write(fd, data, sizeof(StatData)*size);
    if (written == -1) {
        perror("StoreDump: data write failed");
        close(fd);
        return -1;
    }

    // Копируем на диск все части файла, находящиеся в памяти
    if (fsync(fd) == -1) {
        perror("StoreDump: fsync failed");
        close(fd);
        return -1;
    }

    if (close(fd) == -1) {
        perror("StoreDump: close failed");
        return -1;
    }
    
    return (ssize_t)written / sizeof(StatData);
}
