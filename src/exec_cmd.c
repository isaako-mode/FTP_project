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
#include "user_cmd.h"
#define _XOPEN_SOURCE
#define MAX_CMD_OUTPUT 50
#define MAX_USER_OUTPUT_LEN 50
#define MAX_FILE_NAME_LEN 20
// Evaluate the command to ensure it's in the list of existing ones
int eval_cmd(Slice cmd) {

    const char* VALID_CMDS[] = {LIST, INST, NULL};
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


int list(Message* message) {
    //change the command to display the directory
    // memcpy(message->command.data, DSPY, CMD_LEN);
    //Define directory struct
    struct dirent *de;
    DIR *dr;

    char path[message->current_directory.len];
    memcpy(path, message->current_directory.data, message->current_directory.len);
    

    path[message->current_directory.len] = '\0';

    // printf("\nCURRENT PATH: %s\n", message->current_directory.data);

    dr = opendir(message->current_directory.data);

    char hidden = '.';

    // pointer to track the user data memory position as file names are added
    char* next_ptr = message->user_data.data;
    int counter = 0;

    // Reset the user data memory length as we are rewriting it
    message->user_data.len = 0;
    if (dr != NULL) {
        while ((de = readdir(dr)) != NULL) {

            //Skip hidden dirs
            if(strncmp(de->d_name, &hidden, 1) == 0) {
                continue;
            }
            // printf("%s\n", de->d_name);

            // Copy the file name string at the current pointer position
            int len = strlen(de->d_name);
            assert(len < MAX_FILE_NAME_LEN);
            // adding 1 for comma separator
            memcpy(next_ptr, de->d_name, len);

            // Increment the length and the next pointer position
 
            // message->user_data.len += len;
            next_ptr += len;

             // Add newline
            *next_ptr = '\n';
            next_ptr++;

            // Update length
            message->user_data.len += len + 1;
            counter++;
        }

        closedir(dr); // Close the directory stream
    } else {
        // char* error = "Could not open directory";
        // int error_len = strlen(error);
        // perror("Could not open directory");
        // message->user_data.len = error_len;
        // memcpy(message->user_data.data, error, error_len);
        printf("Could not open directory");
        return 400;
    }

    return 200;

}

user_cmd exec_command(Message* message) {
    
    user_cmd input_cmd;

    // for (size_t i = 0; i < message->current_directory.len; i++) {
    //     printf("[%02zu] = 0x%02x (%c)\n", i,
    //         (unsigned char)message->current_directory.data[i],
    //         message->current_directory.data[i] >= 32 &&
    //         message->current_directory.data[i] <= 126 ?
    //         message->current_directory.data[i] : '.');
    // }

    if (eval_cmd(message->command) != 0) {
        input_cmd = CMD_FAIL;
        return input_cmd;
    }

    if (slc_str_cmp(message->command, "LIST") == 0) {
        printf("\nCOMMAND AT LIST: %s\n", message->command.data);
        list(message);
        input_cmd = CMD_LIST;
    }

    for(size_t i; i<message->user_data.len;i++) {
        printf("%c", message->user_data.data[i]);
    }

    printf("OUTPUT: %s", message->user_data.data);


    return input_cmd;
}