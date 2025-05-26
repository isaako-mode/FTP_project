#ifdef SLICE_H
#define SLICE_H

#include <stddef.h>

typedef struct Slice {
    char* data;
    char* offset;
    size_t len;
} Slice;

#endif