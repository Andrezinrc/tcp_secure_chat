#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int client_fd;
char key[16];

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

    // recebe uma mensagem
    int bytes = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if(bytes > 0){
        buffer[bytes] = '\0';
        printf("[Cliente] %s\n", buffer);
    }
    
    close(client_fd);
    close(server_fd);
    
    return 0;
}
