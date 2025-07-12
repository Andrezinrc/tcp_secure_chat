#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdlib.h>

// criptografia
void encrypt_data(char* data, size_t size, const char* key, size_t keySize);

// descriptografia
void decrypt_data(char* data, size_t size, const char* key, size_t keySize);

#endif
