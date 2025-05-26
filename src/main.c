#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "network.h"
#include "parser.h"

#define BUFFER_SIZE 1024

typedef struct Message {
    char command[4];
    char* file_name;
    char* current_directory;
    char** args;

    int response_code;
} Message;

int main() {
    //external function definitions
    char* get_message();
    Message* parse_message(char*);
    void free_message(Message*);
    char* get_resp(char*, int, int);
    int create_server_socket();
    int accept_message(int);
    int send_message(int, const char*);
    int close_connection(int, int);

    //Max size of response message
    char message_buffer[BUFFER_SIZE];
    memset(message_buffer, ' ', BUFFER_SIZE);

    //get message from TCP socket
    // char* message = get_message();
    int server_fd = create_server_socket();
    int client_fd = accept_message(server_fd);
    get_resp(message_buffer, server_fd, client_fd);

    if (strcmp(message_buffer, "400 BAD REQUEST") == 0) {
        fprintf(stderr, "Failed to get message\n");
        send_message(client_fd, "400 BAD REQUEST");
        return 1;
    }

    printf("Message: %s\n", message_buffer);
    
    //parse the message
    //struct that represents the processed message
    Message* parsed_message;
    parsed_message = parse_message(message_buffer);
    
    if (parsed_message->response_code == 400) {
        send_message(client_fd, "400 BAD REQUEST");
        close_connection(server_fd, client_fd);
        return 0;
    }
    //free the original message

    printf("Command: %s\n", parsed_message->command);
    printf("File: %s\n", parsed_message->file_name);
    printf("current_dir: %s\n", parsed_message->current_directory);

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