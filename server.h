#ifndef SERVER_H
#define SERVER_H

#define PORT 8080
#define BUFFER_SIZE 1024

int client_fd;
char key[16];

// protótipos das funções
void* receive_messages(void* arg);
void* send_messages(void* arg);

#endif
