#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "network.h"
#include "parser.h"

typedef struct Message {
    char* command;
    char** args;
    char* file;
} Message;

int main() {
    char* get_message();
    Message* parse_message(char*);

    char* message = get_message();
    if (message == NULL) {
        fprintf(stderr, "Failed to get message\n");
        return 1;
    }

    printf("Message: %s\n", message);
    
    parse_message(message);
    free(message);

    
    return 0;
}