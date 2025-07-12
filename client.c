#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>

#define SERVER_IP "127.0.0.1" // IP
#define PORT 8080 // porta
#define BUFFER_SIZE 1024

int sockfd;

// thread para receber mensagens do servidor
void* receive_messages(void* arg) {
    char buffer[BUFFER_SIZE];

    while (1) {
        int bytes = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes <= 0) break;

        buffer[bytes] = '\0';

        // limpa a linha, imprime a mensagem e reaparece o prompt
        printf("\r[Servidor] %s\n", buffer);
        printf("> ");
        fflush(stdout);
    }
    return NULL;
}

// thread para enviar mensagens digitadas
void* send_messages(void* arg) {
    char buffer[BUFFER_SIZE];

    while (1) {
        printf("> ");
        fflush(stdout);
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) break;
        buffer[strcspn(buffer, "\n")] = 0;

        send(sockfd, buffer, strlen(buffer), 0);
    }
    return NULL;
}

int main() {
    struct sockaddr_in server_addr;
    
    // cria o socket TCP IPv4
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Erro ao criar socket");
        exit(1);
    }

    // configura o endereço do servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // conecta ao servidor usando o endereço configurado
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erro ao conectar");
        exit(1);
    }

    // cria threads
    pthread_t recv_thread, send_thread;
    pthread_create(&recv_thread, NULL, receive_messages, NULL);
    pthread_create(&send_thread, NULL, send_messages, NULL);

    // espera as threads
    pthread_join(recv_thread, NULL);
    pthread_join(send_thread, NULL);
    close(sockfd);

    return 0;
}
