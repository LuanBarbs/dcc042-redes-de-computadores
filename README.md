# DCC042 - Redes de Computadores

## Estrutura

```plaintext
01_SocketEcho/                  # Sockets básicos em C, C++ e Python
02_SocketEchoMultiThread/       # Sockets com suporte a multithread com pool de threads
03_TVC1_EchoSocket_MultiThread  # Sockets com suporte a multithread para o TVC1
.gitignore                      # Arquivos que o Git deve ignorar (como os .exe)
README.md                       # Estrutura do projeto.
```

## TVC 1

Para rodar os códigos da pasta 03_TVC1_EchoSocket_MultiThread basta fazer:
```bash
g++ <nome_codigo.cpp> -o <executavel>

./executavel
```

Em relação ao echo_client.cpp é necessário fazer:
```bash
./echo_client <host>
```
