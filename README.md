# DCC042 - Redes de Computadores

# Echo Server e Echo Client em C (Windows) 🎧

Este repositório implementa um **Echo Server** e um **Echo Client** usando **sockets TCP** em C, compatível com **Windows (Winsock)**.

## Estrutura
01_SocketEcho/
├─ echo_server.c # Código do servidor
├─ echo_client.c # Código do cliente
├─ echo_server.exe # Executável do servidor (ignorado pelo Git)
├─ echo_client.exe # Executável do cliente (ignorado pelo Git)
├─ README.md # Este arquivo
└─ .gitignore # Ignora arquivos compilados e temporários

## Como compilar (Windows)

Requer `gcc` (via MinGW ou WSL com suporte a Windows) com a biblioteca `ws2_32`.

```bash
gcc -o echo_server.exe echo_server.c -lws2_32
gcc -o echo_client.exe echo_client.c -lws2_32
```

## Como usar

1. Em um terminal, inicie o servidor:

```bash
./echo_server.exe
```

2. Em outro terminal, conecte com o cliente:

```bash
./echo_client.exe SeuNome 127.0.0.1
```

Digite mensagens e veja o servidor ecoar as respostas de volta.