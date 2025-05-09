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
    if (argc < 3) {
        cerr << "Uso: ./echo_client <nome> <host>\n";
        return 1;
    }

    string name = argv[1];
    string host = argv[2];

    // Criando o soquete do cliente.
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Erro ao criar socket");
        return 1;
    }

    // Definindo o endereço do servidor.
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    if (inet_pton(AF_INET, host.c_str(), &serverAddress.sin_addr) <= 0) {
        cerr << "Endereço invalido\n" << endl;
        return 1;
    }

    // Conectando ao servidor.
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Erro na conexao");
        return 1;
    }

    cout << "Conectando servidor " << host << ":" << PORT << endl;

    // Coletando a mensagem.
    char buffer[BUFFER_SIZE];
    string msg;
    while (true) {
        cout << "> ";
        getline(cin, msg);
        if (msg == "exit") break;

        string full_msg = "[" + name + "]: " + msg + "\n";

        // Enviando dados para o servidor.
        send(clientSocket, full_msg.c_str(), full_msg.length(), 0);

        ssize_t bytes_received = read(clientSocket, buffer, BUFFER_SIZE);
        if (bytes_received <= 0) break;

        buffer[bytes_received] = '\0';
        cout << "Eco: " << buffer;
    }

    // Fechando o Socket do Cliente.
    cout << "Encerrando a conexao" << endl;
    close(clientSocket);
    return 0;
}