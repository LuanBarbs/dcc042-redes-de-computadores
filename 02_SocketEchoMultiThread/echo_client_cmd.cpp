#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444
#define BUFFER_SIZE 1024

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: ./echo_client_cmd <host>\n";
        return 1;
    }

    string host = argv[1];

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Erro ao criar socket");
        return 1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    if(inet_pton(AF_INET, host.c_str(), &serverAddress.sin_addr) <= 0) {
        cerr << "Endereço invalido\n";
        return 1;
    }

    if(connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Erro na conexao");
        return 1;
    }

    cout << "Conectado ao servidor " << host << ":" << PORT << endl;
    cout << "Use os comandos:\n - echo <mensagem>\n - quit\n";

    char buffer[BUFFER_SIZE];
    string input;

    while (true) {
        cout << "> ";
        getline(cin, input);

        if (input.empty()) continue;

        send(clientSocket, input.c_str(), input.length(), 0);

        if (input == "quit") break;

        ssize_t bytes_received = read(clientSocket, buffer, BUFFER_SIZE - 1);
        if (bytes_received <= 0) break;

        buffer[bytes_received] = '\0';
        cout << "Resposta: " << buffer;
    }

    cout << "Encerrando conexão" << endl;
    close(clientSocket);
    return 0;
}