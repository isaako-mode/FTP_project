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

typedef struct Buffer {
    char* data;
    size_t capacity;
    size_t buf_len;
} Buffer;


typedef struct Slice {
    char* data;
    char* offset;
    size_t len;
} Slice;

typedef struct {
    Slice command;
    Slice file_name;
    Slice current_directory;
    Slice arg1;
    Slice arg2;

    int response_code;

} Message;


void trim_newline(char *str) {
    int index = strlen(str) - 1;
    if (str[index] == '\n') {
      str[index] = '\0';
    }
  }

void free_message(Message* message) {
    // if (message != NULL) {
    //     // free(message->command);
    //     free(message->file_name);
    //     free(message->current_directory);
    //     for (int i = 0; message->args[i] != NULL; i++) {
    //         free(message->args[i]);
    //     }
    //     free(message->args);
    //     free(message);
    // }
}

Message* parse_message(Buffer* buffer) {



    //struct that represents the processed message
    Message* processed_message;
    processed_message = calloc(1, sizeof(Message));
    if (processed_message == NULL) {
        perror("Memory allocation failed for Message type");
        return NULL;
    }
    Slice* slice_map[] = {&processed_message->command, &processed_message->file_name, &processed_message->current_directory, &processed_message->arg1, &processed_message->arg2};
    //Partition the buffer
    int cmd_pos = 0;
    int len_counter = 0;
    slice_map[0]->offset = buffer->data;
    for (size_t i=0; i<buffer->buf_len; i++) {
        
        if (cmd_pos > 5) {
            break;
        }

        slice_map[cmd_pos]->offset += 1;
        //once an inner boundary is reached, set the input values
        if ((buffer->data[i] == '\r' && buffer->data[i+1] == '\n') || (buffer->data[i+1] == '\n' && i == buffer->buf_len-1)) {
            printf("%li\n", i);
            buffer->data[i] = '\0';
            slice_map[cmd_pos]->len = len_counter; 
            slice_map[cmd_pos]->data = slice_map[cmd_pos]->offset - slice_map[cmd_pos]->len;
            len_counter = 0;

            cmd_pos += 1;
            slice_map[cmd_pos]->offset = &buffer->data[i];

        }

        //set the boundary values to null
        if (buffer->data[i] == '\n') {
            buffer->data[i] = '\0';
        }

        len_counter += 1;

    }


    return processed_message;
}