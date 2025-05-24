#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9000
#define BUFFER_SIZE 1024


char* get_message() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        return NULL;
    }

    // Bind to port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return NULL;
    }

    listen(server_fd, 1);
    printf("Listening on port %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("Accept failed");
        return NULL;
    }

    int n = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if (n > 0) {
        buffer[n] = '\0';
        printf("Received: %s\n", buffer);
    } else {
        printf("No data received\n");
    }

    close(client_fd);
    close(server_fd);
    char* resp = malloc(strlen(buffer) + 1);
    if (resp == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    resp = strcpy(resp, buffer);
    if (resp == NULL) {
        perror("String copy failed");
        return NULL;
    }
    return resp;
}

int main() {
    char* message = get_message();
    if (message == NULL) {
        fprintf(stderr, "Failed to get message\n");
        return 1;
    }

    printf("Message: %s\n", message);
    
    return 0;
}