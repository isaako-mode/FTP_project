#ifdef NETWORK_H
#define NETWORK_H


char* get_message();
char* get_resp(char*, int, int);
int create_server_socket();
int accept_message(int);
int send_message(int, const char*);
int close_connection(int, int);

#define PORT 9000
#define BUFFER_SIZE 1024

#endif