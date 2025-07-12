#include <stdlib.h>

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
