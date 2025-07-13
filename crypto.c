#include <stdio.h>
#include <stdlib.h>

#include "colors.h"

// criptografa
void encrypt_data(char* data, size_t size, const char* key, size_t keySize) {
    for (size_t i = 0; i < size; i++) {
        data[i] = (data[i] + key[i % keySize]) % 256;
    }
}

// descriptgrafa
void decrypt_data(char* data, size_t size, const char* key, size_t keySize) {
    for (size_t i = 0; i < size; i++) {
        data[i] = (256 + data[i] - key[i % keySize]) % 256;
    }
}

// função para imprimir dados em hexadecimal OBS: para debug
void print_hex(const char* label, const char* data, size_t len) {
    printf("%s[%s] ", YELLOW, label);
    for (size_t i = 0; i < len; i++) {
        printf("0x%02X ", (unsigned char)data[i]);
    }
    printf(RESET "\n");
}
