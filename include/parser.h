#ifndef PARSER_H
#define PARSER_H
#include "slice.h"
#include "buffer.h"

typedef struct {
    Slice command;
    Slice current_directory;
    Slice file_name;
    Slice user_data;
    Slice arg1;
    Slice arg2;
    Slice file_data;

    int response_code;

} Message;

Message* parse_message(Buffer* buffer);
void trim_newline(char *str);
// void free_message(Message*)

#endif