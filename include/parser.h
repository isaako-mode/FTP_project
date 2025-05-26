#ifdef PARSER_H
#define PARSER_H

typedef struct {
    Slice command;
    Slice file_name;
    Slice current_directory;
    Slice arg1;
    Slice arg2;

    int response_code;

} Message;

Message parse_message(char*);
void trim_newline(char *str);
void free_message(Message*)

#endif