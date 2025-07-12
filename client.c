#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1" // IP
#define PORT 8080 // porta

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char* msg = "Hello World!";

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

    // envia a mensagem para o servidor
    send(sockfd, msg, strlen(msg), 0);
    
    close(sockfd);

    return 0;
}
