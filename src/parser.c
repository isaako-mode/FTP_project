#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"

/*
Gameplan:

tokenize the msg string

if firstArg not in valid_commands -> return "400 BAD REQUEST"

structure: COMMAND,ARGS,FILE_NAME,FILE
*/

typedef struct {
    char* command;
    char** args;

    char* file;

} Message;


Message* parse_message(char* message) {

    //struct that represents the processed message
    Message* processed_message;
    processed_message = malloc(sizeof(Message));
    if (processed_message == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    //tokenize the message string
    char* token = strtok(message, "\r\n");

    int i = 0;
    while(token != NULL) {

        // strs[i] = malloc(strlen(token) + 1);
        // if (strs[i] == NULL) {
        //     perror("failed to allocate memory");
        //     exit(1);
        //  }
         printf("%s", token);
        // strcpy(strs[i], token);
        token = strtok(NULL, " ");
        i += 1;
    }

    return processed_message;
}