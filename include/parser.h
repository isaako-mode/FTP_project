#ifdef PARSER_H
#define PARSER_H

typedef struct Message {
    char* command;
    char** args;
    char* file;
} Message;

Message parse_message(char*);
void trim_newline(char *str);
void free_message(Message*)

#endif