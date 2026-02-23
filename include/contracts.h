#ifndef CONTRACTS_H
#define CONTRACTS_H

#include <assert.h>
#include <sys/types.h>

// Здесь прописаны контракты, на которые опирается реализация
// WARNING: ИЗМЕНЯТЬ ОПАСНО!

#define MAX_STATDATA_ARRAY_SIZE 100000

// `sizeof(off_t) == 8` - стандартно для 64битных систем
static_assert(sizeof(off_t) == 8, "off_t must be 64-bit");

#endif // CONTRACTS_H