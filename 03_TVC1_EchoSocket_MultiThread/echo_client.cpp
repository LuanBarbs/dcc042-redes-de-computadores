#include <iostream>
#include <cstring>          // Para manipulação de strings C.
#include <unistd.h>         // Para close() e read().
#include <sys/socket.h>     // Para socket(), connect().
#include <netinet/in.h>     // Estrutura sockaddr_in.
#include <arpa/inet.h>      // Para inet_pton().

#define SERVERPORT 4444     // Porta padrão do servidor.
#define BUFFER_SIZE 1024    // Tamanho do buffer de leitura.

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        // Exige o IP do servidor como argumento.
        cerr << "Uso: ./echo_client_cmd <host>\n";
        return 1;
    }

    string host = argv[1];  // IP fornecido pelo usuário.

    // Criação do socket TCP (SOCK_STREAM).
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Erro ao criar socket");
        return 1;
    }

    // Configuração do endereço do servidor.
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVERPORT); // Converte para network byte order.

    // Converte string do IP para formato binário.
    if(inet_pton(AF_INET, host.c_str(), &serverAddress.sin_addr) <= 0) {
        cerr << "Endereço invalido\n";
        return 1;
    }

    // Conecta ao servidor.
    if(connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Erro na conexao");
        return 1;
    }

    cout << "Conectado ao servidor " << host << ":" << SERVERPORT << endl;
    cout << "Use os comandos:\n - echo <mensagem>\n - quit\n";

    char buffer[BUFFER_SIZE];   // Buffer para resposta do servidor.
    string input;

    while (true) {
        cout << "> ";
        getline(cin, input);    // Lê entrada do usuário

        if (input.empty()) continue;

        // Envia comando para o servidor.
        send(clientSocket, input.c_str(), input.length(), 0);

        // Encerra se comando for "quit".
        if (input == "quit") break;

        // Lê resposta do servidor.
        ssize_t bytes_received = read(clientSocket, buffer, BUFFER_SIZE - 1);
        if (bytes_received <= 0) break;     // Erro ou desconexão.

        buffer[bytes_received] = '\0';
        cout << "Resposta: " << buffer;     // Exibe resposta.
    }

    cout << "Encerrando conexão" << endl;
    close(clientSocket);    // Fecha o socket.
    return 0;
}