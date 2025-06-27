#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include "crypt.h"
#include "colors.h"
#include "header.h"

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int sockfd;
char key[16];

// gera chave aleatoria e salva em session.key
void generate_key() {
    srand(time(NULL));
    for (int i = 0; i < 16; i++) {
        key[i] = rand() % 256;
    }

    FILE *f = fopen("session.key", "wb");
    if (f) {
        fwrite(key, 1, 16, f);
        fclose(f);
        printf(GREEN "[Cliente] Chave gerada e salva em session.key\n" RESET);
    } else {
        perror(RED "Erro ao salvar a chave" RESET);
        exit(1);
    }
}

// thread para receber mensagens do servidor
void* receive_messages(void* arg) {
    unsigned char encrypted[BUFFER_SIZE];
    char decrypted[BUFFER_SIZE];

    while (1) {
        int bytes = recv(sockfd, encrypted, BUFFER_SIZE, 0);
        if (bytes <= 0) break;

        decrypt_message(encrypted, decrypted, (unsigned char*)key, 16, bytes);

        // limpa a linha, imprime a msg e reaparece o prompt
        printf("\r\033[K" GREEN "[Servidor] %s\n" RESET, decrypted);
        printf("> ");
        fflush(stdout);
    }
    return NULL;
}

// thread para enviar mensagens digitadas
void* send_messages(void* arg) {
    char buffer[BUFFER_SIZE];
    unsigned char encrypted[BUFFER_SIZE];

    while (1) {
        printf("> ");
        fflush(stdout);
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        encrypt_message(buffer, encrypted, (unsigned char*)key, 16);
        send(sockfd, encrypted, strlen(buffer), 0);
    }
    return NULL;
}

int main() {
    print_header("Cliente");

    struct sockaddr_in server_addr;

    // cria socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // configura servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // conecta ao servidor
    connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    printf(GREEN "[Cliente] Conectado ao servidor!\n" RESET);

    // gera chave e envia
    generate_key();
    send(sockfd, key, 16, 0);

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
