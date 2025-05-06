#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")  // Link com Winsock.

#define BUFFER_SIZE 1024
#define PORT 4444  // Porta padrão usada pelo servidor.

int main(int argc, char *argv[]) {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char buffer[BUFFER_SIZE];
    char recv_buffer[BUFFER_SIZE];
    int recv_size;

    if (argc < 3) {
        printf("Uso: %s <nome> <host>\n", argv[0]);
        return 1;
    }

    char *screenName = argv[1];
    char *host = argv[2];

    // Inicializa o Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Erro ao iniciar Winsock: %d\n", WSAGetLastError());
        return 1;
    }

    // Cria socket TCP
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Erro ao criar socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Define estrutura do servidor
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(host);  // converte string IP para binário.

    // Conecta ao servidor.
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Erro ao conectar-se ao servidor\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Conectado ao servidor %s na porta %d\n", host, PORT);

    // Loop de envio/recebimento.
    while (1) {
        printf("Digite sua mensagem (ou 'exit' para sair): ");
        fgets(buffer, BUFFER_SIZE, stdin);  // Lê do teclado.

        // Remove \n do final.
        buffer[strcspn(buffer, "\n")] = 0;

        // Se usuário quiser sair.
        if (strcmp(buffer, "exit") == 0) break;

        // Adiciona nome na frente da mensagem.
        char fullMessage[BUFFER_SIZE];
        snprintf(fullMessage, sizeof(fullMessage), "[%s]: %s", screenName, buffer);

        // Envia mensagem ao servidor.
        send(sock, fullMessage, strlen(fullMessage), 0);

        // Recebe resposta do servidor.
        recv_size = recv(sock, recv_buffer, BUFFER_SIZE - 1, 0);
        if (recv_size > 0) {
            recv_buffer[recv_size] = '\0';  // finaliza string.
            printf("Servidor ecoou: %s\n", recv_buffer);
        }
    }

    // Encerra a conexão.
    printf("Encerrando conexao...\n");
    closesocket(sock);
    WSACleanup();

    return 0;
}