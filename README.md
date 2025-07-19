# TCP Secure Chat

Implementação simples de um chat seguro via TCP em C usando criptografia básica

## Compilação

```bash
gcc server.c crypto.c -o server -lpthread
gcc client.c crypto.c -o client -lpthread
```

## Uso

1. Primeiro, execute o servidor:

```bash
./server
```

2. Depois, execute o cliente em outro terminal ou máquina:

```bash
./client
```

