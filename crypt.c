#include <string.h>

// criptografa a mensagem com a chave e armazena o resultado em output
void encrypt_message(const char* input, unsigned char* output, const unsigned char* key, int keySize) {
    int len = strlen(input);
    for (int i = 0; i < len; i++) {
        output[i] = (input[i] + key[i % keySize]) % 256;
    }
}

// descriptografa a mensagem com a chave e armazena o texto final em output
void decrypt_message(const unsigned char* input, char* output, const unsigned char* key, int keySize, int len) {
    for (int i = 0; i < len; i++) {
        output[i] = (256 + input[i] - key[i % keySize]) % 256;
    }
    output[len] = '\0';
}
