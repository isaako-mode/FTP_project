#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "network.h"
#include "parser.h"
#include "slice.h"
#include "buffer.h"
#include "slice_lib.h"
#include "exec_cmd.h"
#include "user_cmd.h"
#include "build_output.h"

#define BUFFER_SIZE 2048
#define MAX_MSG 4
int main() {

    // Configure the buffer
    Buffer *message_buffer = malloc(sizeof(Buffer));
    if (message_buffer == NULL)
    {
        perror("Failed to allocate memory for the message buffer");
        exit(1);
    }

    message_buffer->capacity = BUFFER_SIZE;
    message_buffer->data = malloc(sizeof(char) * BUFFER_SIZE);
    message_buffer->buf_len = 0;

    // create socket and get server file descriptor
    int server_fd = create_server_socket();

    // listen and accept message from client and get client file descriptor
    int client_fd = accept_message(server_fd);

    Message *parsed_message;
    int max_messages;
    max_messages = 0;

    while (max_messages < 4) {
        // get the response and load it into the message buffer
        get_resp(message_buffer, server_fd, client_fd);

        // message_buffer->data[message_buffer->buf_len] = '\0';
        printf("%s", message_buffer->data);

        // parse the message
        // struct that represents the processed message
        parsed_message = parse_message(message_buffer);

        // Check if the parser function determined if initial input is bad and respond with bad req
        if (parsed_message->response_code >= 400)
        {
            send_message(client_fd, "400 BAD REQUEST");
            // close_connection(server_fd, client_fd);
            return 0;
        }

        for (size_t i = 0; i < parsed_message->command.len; i++)
        {
            printf("%c", parsed_message->command.data[i]);
        }

        printf(" dir name: ");
        for (size_t i = 0; i < parsed_message->current_directory.len; i++)
        {
            printf("%c", parsed_message->current_directory.data[i]);
        }

        printf(" filename: ");
        for (size_t i = 0; i < parsed_message->file_name.len; i++)
        {
            printf("%c", parsed_message->file_name.data[i]);
        }

        printf(" user_data: ");
        for (size_t i = 0; i < parsed_message->user_data.len; i++)
        {
            printf("%c", parsed_message->user_data.data[i]);
        }

        printf("ARG1");
        for (size_t i = 0; i < parsed_message->arg1.len; i++)
        {
            printf("%c", parsed_message->arg1.data[i]);
        }

        printf("ARG2: ");
        for (size_t i = 0; i < parsed_message->arg2.len; i++)
        {
            printf("%c", parsed_message->arg2.data[i]);
        }

        printf(" file_data: ");
        for (size_t i = 0; i < parsed_message->file_data.len; i++)
        {
            printf("%c", parsed_message->file_data.data[i]);
        }

        printf("BEFORE OUTPUT GENERATION");
        for (size_t i = 0; i < message_buffer->buf_len; i++)
        {
            printf("[%02zu] = 0x%02x (%c)\n", i,
                   (unsigned char)message_buffer->data[i],
                   message_buffer->data[i] >= 32 &&
                           message_buffer->data[i] <= 126
                       ? message_buffer->data[i]
                       : '.');
        }

        // User command enum to track what command the user gave
        user_cmd input_cmd;
        input_cmd = exec_command(parsed_message);

        for (size_t i = 0; i < message_buffer->buf_len; i++)
        {
            printf("[%02zu] = 0x%02x (%c)\n", i,
                   (unsigned char)message_buffer->data[i],
                   message_buffer->data[i] >= 32 &&
                           message_buffer->data[i] <= 126
                       ? message_buffer->data[i]
                       : '.');
        }

        configure_output_buff(parsed_message, message_buffer, input_cmd);

        send_message(client_fd, message_buffer->data);
        // max_messages++;

    }
    close_connection(server_fd, client_fd);

    // free_message(parsed_message);
    // free(message_buffer->data);
    free(message_buffer);
    free(parsed_message);

    return 0;
}