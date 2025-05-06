#define _WINSOCK_DEPRECATED_NO_WARNINGS // Uso de funções antigas como inet_noa.
#include <stdio.h>
#include <winsock2.h> // Biblioteca principal para uso de socket no Windows.

#pragma comment(lib, "ws2_32.lib") // Linka automaticamente a biblioteca Winsock durante a compilação

#define PORT 4444
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;                            // Estrutura para armazenar dados da inicialização do Winsock.
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;      // Estruturas com informações de endereço.
    int client_len;
    char buffer[BUFFER_SIZE];
    int recv_size;                          // Quantidade de bytes recebidos.

    // Inicializa a biblioteca Winsock.
    printf("Inicializando Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        // Em caso de falha, exibe código e encerra.
        printf("Falha no WSAStartup. Erro: %d\n", WSAGetLastError());
        return 1;
    }

    // Cria o socket TCP (AF_INET = IPv4, SOCK_STREAM = TCP).
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Erro ao criar socket: %d\n", WSAGetLastError());
        WSACleanup();  // Finaliza o Winsock.
        return 1;
    }

    // Define as propriedades do endereço do servidor.
    server.sin_family = AF_INET;            // IPv4.
    server.sin_addr.s_addr = INADDR_ANY;    // Aceita conexões em qualquer interface local.
    server.sin_port = htons(PORT);          // Converte a porta para o formato de rede (big-endian).

    // Faz o bind do socket à porta e endereço IP definidos.
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Erro no bind: %d\n", WSAGetLastError());
        closesocket(server_socket);  // Fecha o socket.
        WSACleanup();                // Finaliza o Winsock.
        return 1;
    }

    // Começa a escutar conexões com uma fila de no máximo 3 clientes.
    listen(server_socket, 3);
    printf("Servidor iniciado na porta %d...\n", PORT);

    // Loop principal: aceita conexões de clientes continuamente.
    while (1) {
        client_len = sizeof(struct sockaddr_in); // Define o tamanho da estrutura do cliente.

        // Aceita uma conexão (bloqueante até que um cliente se conecte).
        client_socket = accept(server_socket, (struct sockaddr *)&client, &client_len);
        if (client_socket == INVALID_SOCKET) {
            printf("Erro no accept: %d\n", WSAGetLastError());
            continue;  // Volta para esperar outra conexão.
        }

        // Exibe o IP e porta do cliente conectado.
        printf("Conexao aceita de %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        // Loop de echo: recebe dados e os envia de volta.
        while ((recv_size = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
            send(client_socket, buffer, recv_size, 0); // Envia de volta os mesmos dados recebidos.
        }

        // Saiu do loop: cliente desconectou.
        printf("Conexao encerrada\n");
        closesocket(client_socket);  // Fecha o socket do cliente.
    }

    // Nunca chega aqui no loop infinito, mas seria a finalização correta.
    closesocket(server_socket); // Fecha o socket do servidor.
    WSACleanup();               // Libera recursos do Winsock.
    
    return 0;
}