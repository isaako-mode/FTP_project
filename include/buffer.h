#ifdef BUFFER_H
#define BUFFER_H

#include <stddef.h>

typedef struct Buffer {
    char* data;
    size_t capacity;
    size_t buf_len;
} Buffer;

#endif