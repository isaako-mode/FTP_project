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

Message* parse_message(Buffer* buffer) {

    //struct that represents the processed message
    Message* processed_message;
    processed_message = calloc(1, sizeof(Message));
    if (processed_message == NULL) {
        perror("Memory allocation failed for Message type");
        return NULL;
    }

    // Check if the message ends and starts with \n
    if (buffer->data[0] != '\n' || buffer->data[buffer->buf_len - 1] != '\n') {
        processed_message->response_code = 400;
        return processed_message;
    }
    
    // used to easily access the slices by index when partitioning the buffer
    Slice* slice_map[] = {&processed_message->command, &processed_message->file_name, &processed_message->current_directory, &processed_message->arg1, &processed_message->arg2};
    
    //setup vars: 
        //cmd_pos to track current command being parsed
        //len_counter to track the length of each slice
        //initialize the offset at the start of the message buffer to count
    int cmd_pos = 0;
    int len_counter = 0;
    slice_map[0]->offset = buffer->data;

    //Partition the buffer by \r\n to extract the user inputs
    for (size_t i=0; i<buffer->buf_len; i++) {
        
        // make sure we don't process more than max commands
        if (cmd_pos > 4) {
            break;
        }

        slice_map[cmd_pos]->offset += 1;
        //once an inner boundary is reached, set the input values
        if ((buffer->data[i] == '\r' && buffer->data[i+1] == '\n') || (buffer->data[i] == '\n' && i == buffer->buf_len-1)) {
            
            // Ensure the iterator doesn't exceed the buffer size before accessing the memory block
            assert(i < buffer->buf_len);
            // Configure the slices (length, data, offset)
            buffer->data[i] = '\0';
            slice_map[cmd_pos]->len = len_counter; 
            slice_map[cmd_pos]->data = slice_map[cmd_pos]->offset - slice_map[cmd_pos]->len;
            len_counter = 0;

            if (cmd_pos >= 4) {
                break;
            }
            // Increment to next command and set its offset
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