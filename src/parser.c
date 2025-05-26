#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#include <assert.h>
#define MAX_CMDS 6

#define BAD_REQUEST 400
#define SUCESS 200
/*
Gameplan:

tokenize the msg string

if firstArg not in valid_commands -> return "400 BAD REQUEST"

structure: COMMAND,ARGS,FILE_NAME,FILE
*/

typedef struct {
    char command[4];
    char* file_name;
    char* current_directory;
    char** args;

    int response_code;

} Message;

void trim_newline(char *str) {
    int index = strlen(str) - 1;
    if (str[index] == '\n') {
      str[index] = '\0';
    }
  }

void free_message(Message* message) {
    if (message != NULL) {
        // free(message->command);
        free(message->file_name);
        free(message->current_directory);
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
        perror("Memory allocation failed for Message type");
        return NULL;
    }

    char** str_array;
    str_array = malloc(sizeof(char*) * (MAX_CMDS + 1));
    if(str_array == NULL) {
        perror("failed to allocate memory str_array");
        exit(1);
    }

    //tokenize the message string
    char* token = strtok(message, "\r\n");

    int num_cmds = 0;
    while(token != NULL && num_cmds < MAX_CMDS) {

        str_array[num_cmds] = malloc(strlen(token) + 1);
        if (str_array[num_cmds] == NULL) {
            perror("failed to allocate memory for str_array member");
            exit(1);
         }
        strcpy(str_array[num_cmds], token);
        token = strtok(NULL, "\r\n");
        num_cmds += 1;
    }

    if(str_array[0] != NULL) {
        memset(processed_message->command, 0, 4);
        // processed_message->command = malloc(strlen(str_array[0]) + 1);
        // if (processed_message->command == NULL) {
        //     perror("failed to allocate memory");
        //     exit(1);
        // }
    }
    else {
        processed_message->response_code = BAD_REQUEST;
    }

    if(str_array[1] != NULL) {
        processed_message->file_name = malloc(strlen(str_array[1]) + 1);
        if (processed_message->file_name == NULL) {
            perror("failed to allocate memory for file name");
            exit(1);
        }
    }

    if(str_array[2] != NULL) {
        processed_message->current_directory = malloc(strlen(str_array[3]) + 1);
        if (processed_message->current_directory == NULL) {
            perror("failed to allocate memory for dir name");
            exit(1);
        }
    }

    processed_message->args = malloc(sizeof(char*) * (num_cmds - 2) + 1);
            if (processed_message->args == NULL) {
                perror("failed to allocate memory for arg");
                exit(1);
            }

    for(int i=0; i<num_cmds; i++) {
        //trim_newline(str_array[i]);
        if (i == 0) {

            //for handling cmd (only allocated 4)
            if (strlen(str_array[i]) == 4 && str_array[i] != NULL) {
                memcpy(processed_message->command, str_array[i], 4);
                processed_message->response_code = SUCESS;
            }

            else {
                processed_message->response_code = BAD_REQUEST;
                break;
            }
        } 
        else if (i == 1) {
            strcpy(processed_message->file_name, str_array[i]);
        } 
        else if (i == 2) {
            strcpy(processed_message->current_directory, str_array[i]);
        }

        else {
            processed_message->args[i-3] = malloc(strlen(str_array[i]) + 1);
            if (processed_message->args[i-3] == NULL) {
                perror("failed to allocate memory for message arg member");
                exit(1);
            }
            strcpy(processed_message->args[i-3], str_array[i]);
        }
    }

    for (int i = 0; i < num_cmds; i++) {
        free(str_array[i]);
    }
    free(str_array);

    //ensure response type does exceed max message size
    // assert(processed_message->response_code != NULL);
    return processed_message;
}