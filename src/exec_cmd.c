#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include<fcntl.h> 

// Evaluate the command to ensure it's in the list of existing ones
int eval_cmd(char* cmd) {

    const char* VALID_CMDS[] = {"LIST", "PLACE", "FILE", NULL};
    int is_valid = 1;

    if (strlen(cmd) > 4) {
        return 1;
    }

    for (int i=0; VALID_CMDS[i] != NULL; i++) {

        if(strcmp(cmd, VALID_CMDS[i]) == 0) {
            is_valid = 1;
            break;
        }
    }
    return is_valid;
}


char* list(char** args) {

    //Define directory struct
    struct dirent *de;
    DIR *dr;

    if(args[1] == NULL) {
        dr = opendir(".");
    } 
    else if(strcmp(args[1], "~") == 0) {
        dr = opendir("./user");
    }

    else {
        dr = opendir(args[1]);
    }

    if (dr == NULL) {
        printf("Could not open current directory" ); 
        return 1;
    }

    char hidden = '.';
    while ((de = readdir(dr)) != NULL) {
        //SEG FAULT HERE!!
        if(strncmp(de->d_name, &hidden, 1) == 0) {
            continue;
        }

        printf("%s  ", de->d_name); 
    }

    closedir(dr);
    return 0; 
}