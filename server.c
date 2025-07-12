#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "server.h"
#include "crypto.h"

// thread para receber mensagens do cliente
void* receive_messages(void* arg) {
    char buffer[BUFFER_SIZE];

    while (1) {
        int bytes = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes <= 0) break;

        // descriptografa a mensagem
        decrypt_data(buffer, bytes, key, 16);
        
        // apaga a linha atual e imprime a mensagem recebida,
        // depois repõe o prompt
        buffer[bytes] = '\0';
        printf("\r[Cliente] %s\n", buffer);
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

        // criptografa a mensagem
        encrypt_data(buffer, strlen(buffer), key, 16);

        // envia a mensagem
        send(client_fd, buffer, strlen(buffer), 0);
    }
    return NULL;
}

int main(){
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_size = sizeof(client_addr);
    char buffer[BUFFER_SIZE]; 

    // cria socket TCP
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1){
        perror("Erro ao criar socket");
        exit(1);
    }

    // define informações do servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT); // define a porta
    server_addr.sin_addr.s_addr = INADDR_ANY; // aceita qualquer IP

    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Erro no bind");
        exit(1);
    }

    // espera conexões(max 1 cliente)
    if(listen(server_fd, 1) < 0){
        perror("Erro no listen");
        exit(1);
    }

    printf("[Servidor] Aguardando conexão na porta %d...\n", PORT);
    
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_size);  // aceita cliente
    if (client_fd < 0) {
        perror("Erro no accept");
        exit(1);
    }
    printf("[Servidor] Cliente conectado!\n");

    // recebe a chave enviada pelo cliente
    recv(client_fd, key, 16, 0);
    printf("[Servidor] Chave recebida do cliente!\n");

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
