#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "network.h"
#include "parser.h"

typedef struct Message {
    char* command;
    char* file_name;
    char** args;
} Message;

int main() {
    //external function definitions
    char* get_message();
    Message* parse_message(char*);
    void free_message(Message*);

    //get message from TCP socket
    char* message = get_message();
    if (message == NULL) {
        fprintf(stderr, "Failed to get message\n");
        return 1;
    }

    printf("Message: %s\n", message);
    
    //parse the message
    //struct that represents the processed message
    Message* parsed_message;
    parsed_message = parse_message(message);
    
    //free the original message
    free(message);

    printf("Command: %s\n", parsed_message->command);
    printf("File: %s\n", parsed_message->file_name);
    printf("Arguments: ");
    for (int i = 0; parsed_message->args[i] != NULL; i++) {
        printf("%s ", parsed_message->args[i]);
    }
    printf("\n");

    free_message(parsed_message);


    return 0;
}