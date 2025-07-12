#ifndef CLIENT_H
#define CLIENT_H

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int sockfd;
char key[16];

// gera uma chave aleatória e salva em "session.key"
void generate_key();

// thread para receber mensagens do servidor
void* receive_messages(void* arg);

// thread para enviar mensagens digitadas ao servidor
void* send_messages(void* arg);

#endif
