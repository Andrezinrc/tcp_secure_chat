#ifndef CRYPT_H
#define CRYPT_H

// criptografa a mensagem com a chave e armazena o resultado em output
void encrypt_message(const char* input, unsigned char* output, const unsigned char* key, int keySize);

// descriptografa a mensagem com a chave e armazena o texto final em output
void decrypt_message(const unsigned char* input, char* output, const unsigned char* key, int keySize, int len);

#endif
