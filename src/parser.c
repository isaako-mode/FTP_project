#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#define MAX_CMDS 5
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


int parse_message(char* message) {

    //struct that represents the processed message
    Message* processed_message;
    processed_message = malloc(sizeof(Message*));
    if (processed_message == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    char** str_array;
    str_array = malloc(sizeof(char*) * MAX_CMDS);
    if(str_array == NULL) {
        perror("failed to allocate memory");
        exit(1);
    }

    //tokenize the message string
    char* token = strtok(message, "\r\n");

    int num_cmds = 0;
    while(token != NULL || num_cmds > MAX_CMDS) {

        str_array[num_cmds] = malloc(strlen(token) + 1);
        if (str_array[num_cmds] == NULL) {
            perror("failed to allocate memory");
            exit(1);
         }
        printf("%s", token);
        strcpy(strs[i], token);
        token = strtok(NULL, " ");
        num_cmds += 1;
    }

    for(int i=0; i<num_cmds; i++) {
        printf("%s", str_array[i]);
    }

    return 0;
}