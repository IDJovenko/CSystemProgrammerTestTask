#include "serializer/load_dump.h"
#include "serializer/dump_header.h"
#include "contracts.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

// Чтение данных в цикле, с учетом возможности прерывания (EINTR) и частичного чтения
static ssize_t ReadFull(int fd, void *buffer, size_t bytesToRead)
{
    size_t totalRead = 0;

    while (totalRead < bytesToRead) {
        ssize_t chunk = read(fd, (char *)buffer + totalRead, bytesToRead - totalRead);
        if (chunk == 0) {
            break;
        }

        if (chunk == -1) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }

        totalRead += (size_t)chunk;
    }

    return (ssize_t)totalRead;
}

ssize_t LoadDump(StatData** data, const char* filename)
{
    if (data == NULL || filename == NULL) {
        errno = EINVAL;
        perror("LoadDump");
        return -1;
    }

    *data = NULL;

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("LoadDump: open failed");
        return -1;
    }

    DumpHeader header;
    ssize_t headerRead = ReadFull(fd, &header, sizeof(DumpHeader));
    if (headerRead == -1) {
        perror("LoadDump: header read failed");
        close(fd);
        return -1;
    }

    size_t arraySize = header.headerContent.array_size;
    if (arraySize > MAX_STATDATA_ARRAY_SIZE) {
        errno = ERANGE;
        perror("LoadDump: array size exceeds MAX_STATDATA_ARRAY_SIZE");
        close(fd);
        return -1;
    }

    size_t bytesToLoad = sizeof(StatData) * arraySize;
    StatData *loaded = NULL;
    if (arraySize > 0) {
        loaded = (StatData *)malloc(bytesToLoad);
        if (loaded == NULL) {
            perror("LoadDump: memory allocation failed");
            close(fd);
            return -1;
        }

        ssize_t bytesRead = ReadFull(fd, loaded, bytesToLoad);
        if (bytesRead == -1) {
            perror("LoadDump: data read failed");
            free(loaded);
            close(fd);
            return -1;
        }
        if ((size_t)bytesRead != bytesToLoad) {
            errno = EIO;
            perror("LoadDump: unexpected end of file while reading data");
            free(loaded);
            close(fd);
            return -1;
        }
    }

    if (close(fd) == -1) {
        perror("LoadDump: close failed");
        free(loaded);
        return -1;
    }

    *data = loaded;
    return (ssize_t)(bytesToLoad/sizeof(StatData));
}
