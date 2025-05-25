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
    char* get_resp(int, int);
    int create_server_socket();
    int accept_message(int);
    int send_message(int, const char*);
    int close_connection(int, int);



    //get message from TCP socket
    // char* message = get_message();
    int server_fd = create_server_socket();
    int client_fd = accept_message(server_fd);
    char* message = get_resp(server_fd, client_fd);

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

    send_message(client_fd, "200 OK");
    close_connection(server_fd, client_fd);


    free_message(parsed_message);

    return 0;
}