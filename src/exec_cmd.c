#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include <assert.h>
#include<fcntl.h> 
#include "slice_lib.h"
#include "parser.h"
#include "slice.h"
#include "buffer.h"
#include "command_names.h"
#define MAX_CMD_OUTPUT 50
#define MAX_USER_OUTPUT_LEN 50
#define MAX_FILE_NAME_LEN 20

// int slc_str_cmp(Slice slc, const char* str);


/* 
typedef struct output {
    Slice cmd
    Slice 
}

while(de->d_name !- NULL)
    assert(file_name < MAX_FILE_NAME)
    buffer_ptr = output_buffer

    size_t len = strlen(de->d_name)
    memcpy(buffer_pointer, de->d_name, len)

    buffer_ptr += len

*/

// Evaluate the command to ensure it's in the list of existing ones
int eval_cmd(Slice cmd) {

    const char* VALID_CMDS[] = {"LIST", "INST", "FILE", NULL};
    int is_valid = 1;


    for (int i=0; VALID_CMDS[i] != NULL; i++) {

        // if(strcmp(cmd, VALID_CMDS[i]) == 0) {
        if (slc_str_cmp(cmd, VALID_CMDS[i]) == 0) {
            is_valid = 0;
            break;
        }
    }
    return is_valid;
}


void list(Message* message) {
    //change the command to display the directory
    memcpy(message->command.data, DSPY, CMD_LEN);
    //Define directory struct
    struct dirent *de;
    DIR *dr;
    char* root_path = "./user_sys";
    int root_path_len = strlen(root_path);
    
    // assert(memcmp(message->user_data.data, root_path, root_path_len) == 0);
    // char* user_root = malloc(sizeof(char) * (root_path, message->current_directory.len));
    // if (user_root == NULL) {
    //     perror("malloc failed for user root");
    //     exit(1);
    // }

    // memcpy();
    printf("DIRECTOY: %s\n", message->current_directory.data);
    dr = opendir(message->current_directory.data);

    char hidden = '.';

    // pointer to track the user data memory position as file names are added
    char* next_ptr = message->user_data.data;

    // Reset the user data memory length as we are rewriting it
    message->user_data.len = 0;
    if (dr != NULL) {
        while ((de = readdir(dr)) != NULL) {

            //Skip hidden dirs
            if(strncmp(de->d_name, &hidden, 1) == 0) {
                continue;
            }
            printf("%s\n", de->d_name);

            // Copy the file name string at the current pointer position
            int len = strlen(de->d_name);
            assert(len < MAX_FILE_NAME_LEN);

            // adding 1 for comma separator
            memcpy(next_ptr, de->d_name, len);

            // Increment the length and the next pointer position
            message->user_data.len += len;
            next_ptr += len;
        }
        closedir(dr); // Close the directory stream
    } else {
        char* error = "Could not open directory";
        int error_len = strlen(error);
        perror("Could not open directory");
        message->user_data.len = error_len;
        memcpy(message->user_data.data, error, error_len);
    }

}

char* exec_command(Message* message, Buffer* message_buffer) {
    if (eval_cmd(message->command) != 0) {
        return "400 BAD REQUEST";
    }

    if (slc_str_cmp(message->command, "LIST") == 0) {
        list(message);
    }

    printf("OUTPUT: %s", message->user_data.data);


    return "200 OK";
}