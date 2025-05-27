#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "network.h"
#include <assert.h>
#include "buffer.h"

#define PORT 8000
// #define BUFFER_SIZE 1024

//create and bind the socket
int create_server_socket() {
    int server_fd;
    struct sockaddr_in server_addr;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        return 1;
    }

    // Bind to port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
    }

    //Bind the socket with fd
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    return server_fd;
}

//listen and accept incoming client messages
int accept_message(int server_fd) {
    int client_fd;
    struct sockaddr client_addr;
    socklen_t client_len = sizeof(client_addr);

    //Wait and listen to the message
    listen(server_fd, 1);
    printf("Listening on port %d...\n", PORT);

    //Accept the message
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("Accept failed");
        return 1;
    }

    return client_fd;

}

//get response from the client and return it
void get_resp(Buffer* buffer, int server_fd, int client_fd) {

    //Recieve the message and load into buffer
    buffer->buf_len = recv(client_fd, buffer->data, buffer->capacity - 1, 0);
    if (buffer->buf_len > 0) {
        // buffer.data[n] = '\0';

    } else {
        printf("No data received\n");
        buffer->data = "400 BAD REQUUEST";
    }

}

int send_message(int client_fd, const char* message) {
    int sent = send(client_fd, message, strlen(message), 0);
    if (sent < 0) {
        perror("Send failed");
        return -1;
    }
    return 0;
}

int close_connection(int server_fd, int client_fd) {

    if (close(client_fd) > 0) {
        perror("client_fd close failed");
        exit(1);
    }

    if (close(server_fd) > 0) {
        perror("server_fd close failed");
        exit(1);
    }

    printf("%s", "Successfully closed message");

    return 0;
}
