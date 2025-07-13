#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdlib.h>

// criptografia
void encrypt_data(char* data, size_t size, const char* key, size_t keySize);

// descriptografia
void decrypt_data(char* data, size_t size, const char* key, size_t keySize);

// função para imprimir dados em hexadecimal OBS: para debug
void print_hex(const char* label, const char* data, size_t len);
#endif
