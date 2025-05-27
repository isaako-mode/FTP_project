#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include<fcntl.h> 
#include "slice_lib.h"
#include "parser.h"
#include "slice.h"
#include "buffer.h"
#define MAX_CMD_OUTPUT 50

// int slc_str_cmp(Slice slc, const char* str);


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


char* list(Slice current_dir) {

    //Define directory struct
    struct dirent *de;
    DIR *dr;

    dr = opendir("./user_sys");

    // if(message->arg1.data == NULL) {
    //     dr = opendir("./");
    // } 
    // else if(strcmp(args[1], "~") == 0) {
    //     dr = opendir("./user");
    // }

    // else {
    //     dr = opendir(args[1]);
    // }

    // if (dr == NULL) {
    //     printf("Could not open current directory" ); 
    //     return NULL;
    // }
    char* output_buffer = malloc(sizeof(char*) * MAX_CMD_OUTPUT);
    char* basePtr = output_buffer;

    char hidden = '.';
    if (dr != NULL) {
        while ((de = readdir(dr)) != NULL) {
            
            //Skip hidden dirs
            if(strncmp(de->d_name, &hidden, 1) == 0) {
                continue;
            }
            printf("%s\n", de->d_name);

            output_buffer = strdup(de->d_name);
            if (output_buffer != NULL) {
                
            output_buffer++;
            }
        }
        closedir(dr); // Close the directory stream
    } else {
        perror("Could not open directory");
        return NULL;
    }

    return basePtr; 
}

char* exec_command(Message* message) {
    char* resp;
    if (eval_cmd(message->command) != 0) {
        return "400 BAD REQUEST";
    }

    if (slc_str_cmp(message->command, "LIST") == 0) {
        resp = list(message->command);
    }

    char* resp_str = malloc(sizeof(char*) * MAX_CMD_OUTPUT);
    int len = 0;
    for (int i=0; resp[i] != '\0'; i++) {
        len += strlen((char*)resp[i]);
    }


    return "200 OK";
}