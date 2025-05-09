# DCC042 - Redes de Computadores

## Estrutura

```plaintext
01_SocketEcho/
├── echo_server.c       # Código-fonte do servidor em C (Windows)
├── echo_client.c       # Código-fonte do cliente em C (Windows)
├── echo_server.cpp     # Código-fonte do servidor em C++ (Linux/WSL)
├── echo_client.cpp     # Código-fonte do cliente em C++ (Linux/WSL)
├── echo_server.py      # Código-fonte do servidor em python
├── echo_client.py      # Código-fonte do cliente em python
├── .gitignore          # Arquivos que o Git deve ignorar (como os .exe)
└── README.md           # Explicação e instruções do projeto
```

## Echo Server e Echo Client em C (Windows) 🎧

Este repositório implementa um **Echo Server** e um **Echo Client** usando **sockets TCP** em C, compatível com **Windows (Winsock)**.

### Como compilar (Windows)

Requer `gcc` (via MinGW ou WSL com suporte a Windows) com a biblioteca `ws2_32`.

```bash
gcc -o echo_server.exe echo_server.c -lws2_32
gcc -o echo_client.exe echo_client.c -lws2_32
```

### Como usar

1. Em um terminal, inicie o servidor:

```bash
./echo_server.exe
```

2. Em outro terminal, conecte com o cliente:

```bash
./echo_client.exe SeuNome 127.0.0.1
```

Digite mensagens e veja o servidor ecoar as respostas de volta.

## Echo Server e Client em C++ (Linux/WSL) 🧪
Versão utilizando sys/socket.h, compatível com sistemas Unix-like.

### Como compilar (Linux/WSL)
```bash
g++ -o echo_server echo_server.cpp
g++ -o echo_client echo_client.cpp
```

### Como rodar

1. Em um terminal, inicie o servidor:

```bash
./echo_server
```

2. Em outro:
```bash
./echo_client SeuNome 127.0.0.1
```

## Versão em Python 🐍

### Como rodar

```bash
# Em um terminal
python echo_server.py

# Em outro terminal
python echo_client.py SeuNome 127.0.0.1
```