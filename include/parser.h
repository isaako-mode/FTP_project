#ifndef PARSER_H
#define PARSER_H
#include "slice.h"
#include "buffer.h"

typedef struct {
    Slice command;
    Slice file_name;
    Slice current_directory;
    Slice arg1;
    Slice arg2;

    int response_code;

} Message;

Message* parse_message(Buffer* buffer);
void trim_newline(char *str);
// void free_message(Message*)

#endif