#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "network.h"
#include "parser.h"
#include "slice.h"
#include "buffer.h"
#pragma message("Included buffer.h")

#define BUFFER_SIZE 1024

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

typedef struct Message {
    Slice command;
    Slice file_name;
    Slice current_directory;
    Slice arg1;
    Slice arg2;

    int response_code;

} Message;

int main() {
    //external function definitions
    char* get_message();
    Message* parse_message(Buffer*);
    void free_message(Message*);
    char* get_resp(Buffer*, int, int);
    int create_server_socket();
    int accept_message(int);
    int send_message(int, const char*);
    int close_connection(int, int);

    //Max size of response message
    Buffer* message_buffer = malloc(sizeof(Buffer));
    message_buffer->capacity = BUFFER_SIZE;
    message_buffer->data = malloc(sizeof(char) * BUFFER_SIZE);
    message_buffer->buf_len = 0;

    // memset(message_buffer.data, ' ', BUFFER_SIZE);

    //get message from TCP socket
    // char* message = get_message();
    int server_fd = create_server_socket();
    int client_fd = accept_message(server_fd);
    get_resp(message_buffer, server_fd, client_fd);

    // message_buffer->data[message_buffer->buf_len] = '\0';
    printf("%s", message_buffer->data);

    // if (strcmp(message_buffer, "400 BAD REQUEST") == 0) {
    //     fprintf(stderr, "Failed to get message\n");
    //     send_message(client_fd, "400 BAD REQUEST");
    //     return 1;
    // }

    // //parse the message
    // //struct that represents the processed message
    Message* parsed_message;
    parsed_message = parse_message(message_buffer);
    printf("%li", parsed_message->file_name.len);

    for (int i = 0; i < parsed_message->file_name.len;i++) {
        printf("%c\n", parsed_message->file_name.data[i]);
    }

    // printf("%c\n", parsed_message->file_name.data[1]);
    
    // if (parsed_message->response_code == 400) {
    //     send_message(client_fd, "400 BAD REQUEST");
    //     close_connection(server_fd, client_fd);
    //     return 0;
    // }
    // //free the original message

    // printf("Command: %s\n", parsed_message->command);
    // printf("File: %s\n", parsed_message->file_name);
    // printf("current_dir: %s\n", parsed_message->current_directory);

    // printf("Arguments: ");
    // for (int i = 0; parsed_message->args[i] != NULL; i++) {
    //     printf("%s ", parsed_message->args[i]);
    // }
    // printf("\n");

    send_message(client_fd, "200 OK");
    close_connection(server_fd, client_fd);


    // free_message(parsed_message);
    free(message_buffer->data);
    free(message_buffer);

    return 0;
}