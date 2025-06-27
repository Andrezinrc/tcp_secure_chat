#include <stdio.h>
#include "colors.h"

// imprime o cabecalho decorativo
void print_header(const char* role) {
    printf("\033[2J\033[H");
    printf(BLUE "=====================================\n" RESET);
    printf(BLUE "      TCP Secure Chat - %s\n" RESET, role);
    printf("         Andrecode ©2025\n");
    printf(BLUE "=====================================\n\n" RESET);
}
