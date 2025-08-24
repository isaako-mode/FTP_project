#ifndef RESPONSE_MESSAGE_H
#define RESPONSE_MESSAGE_H
#include "slice.h"
#include "buffer.h"

typedef struct {
    Slice response_code;
    Slice data;

} ResponseMessage;

#endif