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
    char* file_name;
    char** args;

} Message;

void trim_newline(char *str) {
    int index = strlen(str) - 1;
    if (str[index] == '\n') {
      str[index] = '\0';
    }
  }

void free_message(Message* message) {
    if (message != NULL) {
        free(message->command);
        free(message->file_name);
        for (int i = 0; message->args[i] != NULL; i++) {
            free(message->args[i]);
        }
        free(message->args);
        free(message);
    }
}

Message* parse_message(char* message) {

    //struct that represents the processed message
    Message* processed_message;
    processed_message = malloc(sizeof(Message));
    if (processed_message == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    char** str_array;
    str_array = malloc(sizeof(char*) * (MAX_CMDS + 1));
    if(str_array == NULL) {
        perror("failed to allocate memory");
        exit(1);
    }

    //tokenize the message string
    char* token = strtok(message, "\r\n");

    int num_cmds = 0;
    while(token != NULL && num_cmds < MAX_CMDS) {

        str_array[num_cmds] = malloc(strlen(token) + 1);
        if (str_array[num_cmds] == NULL) {
            perror("failed to allocate memory");
            exit(1);
         }
        strcpy(str_array[num_cmds], token);
        token = strtok(NULL, "\r\n");
        num_cmds += 1;
    }

    if(str_array[0] != NULL) {
        processed_message->command = malloc(strlen(str_array[0]) + 1);
        if (processed_message->command == NULL) {
            perror("failed to allocate memory");
            exit(1);
        }
    }

    if(str_array[1] != NULL) {
        processed_message->file_name = malloc(strlen(str_array[1]) + 1);
        if (processed_message->file_name == NULL) {
            perror("failed to allocate memory");
            exit(1);
        }
    }

    processed_message->args = malloc(sizeof(char*) * (num_cmds - 2) + 1);
            if (processed_message->args == NULL) {
                perror("failed to allocate memory");
                exit(1);
            }

    for(int i=0; i<num_cmds; i++) {
        //trim_newline(str_array[i]);

        if(i == 0) {
            strcpy(processed_message->command, str_array[i]);
        } 
        else if(i == 1) {
            strcpy(processed_message->file_name, str_array[i]);
        } 
        else {
            processed_message->args[i-2] = malloc(strlen(str_array[i]) + 1);
            if (processed_message->args[i-2] == NULL) {
                perror("failed to allocate memory");
                exit(1);
            }
            strcpy(processed_message->args[i-2], str_array[i]);
        }
    }

    for (int i = 0; i < num_cmds; i++) {
        free(str_array[i]);
    }
    free(str_array);

    return processed_message;
}