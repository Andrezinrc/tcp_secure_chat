#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>

#include "client.h"
#include "crypto.h"
#include "colors.h"

void generate_key() {
    srand(time(NULL));
    for (int i = 0; i < 16; i++) {
        key[i] = rand() % 256;
    }

    FILE *f = fopen("session.key", "wb");
    if (f) {
        fwrite(key, 1, 16, f);
        fclose(f);
        printf("[Cliente] Chave gerada e salva em session.key\n");
    } else {
        perror("Erro ao salvar a chave");
        exit(1);
    }
}

void* receive_messages(void* arg) {
    char buffer[BUFFER_SIZE];

    while (1) 
    {
        int bytes = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes <= 0) break;

        buffer[bytes] = '\0';

        //print_hex("CRYPTO ← RECEBIDA", buffer, bytes);
        decrypt_data(buffer, bytes, key, 16);

        // limpa a linha, imprime a mensagem e reaparece o prompt
        printf(GREEN "\r[Servidor] %s\n" RESET, buffer);
        printf("> ");
        fflush(stdout);
    }
    return NULL;
}

void* send_messages(void* arg) {
    char buffer[BUFFER_SIZE];

    while (1) {
        printf(CYAN "> " RESET);
        fflush(stdout);
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) break;
        buffer[strcspn(buffer, "\n")] = 0;

        // criptografa antes de enviar
        encrypt_data(buffer, strlen(buffer), key, 16);
        //print_hex("CRYPTO → ENVIADO", buffer, strlen(buffer));
        send(sockfd, buffer, strlen(buffer), 0);
    }
    return NULL;
}

int main() {
    printf(BLUE "CLIENTE\n" RESET);
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

    // gera a chave e envia para o servidor
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
