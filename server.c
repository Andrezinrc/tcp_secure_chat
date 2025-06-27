#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "crypt.h"

#define PORT 8080
#define BUFFER_SIZE 1024

int client_fd;
char key[16];

void* receive_messages(void* arg) {
    unsigned char encrypted[BUFFER_SIZE];
    char decrypted[BUFFER_SIZE];

    while (1) {
        int bytes = recv(client_fd, encrypted, BUFFER_SIZE, 0);
        if (bytes <= 0) break;

        decrypt_message(encrypted, decrypted, (unsigned char*)key, 16, bytes);
        printf("\n[Cliente] %s\n", decrypted); // decrypt
    }
    return NULL;
}

void* send_messages(void* arg) {
    char buffer[BUFFER_SIZE];
    unsigned char encrypted[BUFFER_SIZE];

    while (1) {
        printf("> ");
        fflush(stdout);
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // remove \n

        encrypt_message(buffer, encrypted, (unsigned char*)key, 16);
        send(client_fd, encrypted, strlen(buffer), 0); // encrypt
    }
    return NULL;
}

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_size = sizeof(client_addr);

    // cria socket e configura
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 1);
    printf("[Servidor] Aguardando conexão na porta %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_size);
    printf("[Servidor] Cliente conectado!\n");

    // recebe chave
    recv(client_fd, key, 16, 0);
    FILE *kf = fopen("session.key", "wb");
    fwrite(key, 1, 16, kf);
    fclose(kf);
    printf("[Servidor] Chave recebida e salva.\n");

    // cria threads de envio e recebimento
    pthread_t recv_thread, send_thread;
    pthread_create(&recv_thread, NULL, receive_messages, NULL);
    pthread_create(&send_thread, NULL, send_messages, NULL);

    // espera as threads terminarem
    pthread_join(recv_thread, NULL);
    pthread_join(send_thread, NULL);

    close(client_fd);
    close(server_fd);
    return 0;
}
