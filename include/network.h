#ifndef NETWORK_H
#define NETWORK_H
#include "buffer.h"

void get_resp(Buffer* buffer, int server_fd, int client_fd);
int create_server_socket();
int accept_message(int);
int send_message(int, const char*);
int close_connection(int, int);

#define PORT 8000
#define BUFFER_SIZE 1024

#endif