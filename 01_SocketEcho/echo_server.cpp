#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 4444
#define BUFFER_SIZE 1024

using namespace std;

int main() {
    // Criando o socket do servidor.
    // AF_INET: Especifica a família de protocolos IPv4.
    // SOCK_STREAM: Define que o tipo de socket é TCP.
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Erro ao criar o socket");
        return 1;
    }

    // Definindo o endereço do servidor.
    sockaddr_in serverAddress;                      // Tipo de dado usado para armazenar o endereço do socket.
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);           // Função usada para converter unsigned int de bytes da máquina para bytes da rede.
    serverAddress.sin_addr.s_addr = INADDR_ANY;     // Utilizado para não vincular nosso socket a nenhum IP específico. Faz o socket escutar todos os IPs disponíveis.

    // Vinculando o socket do servidor.
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Erro no bind");
        close(serverSocket);
        return 1;
    }

    // Ouvindo Conexões.
    if (listen(serverSocket, 5) < 0) {
        perror("Erro no listen");
        close(serverSocket);
        return 1;
    }

    cout << "Servidor iniciando na porta: " << PORT << endl;

    // Aceitando uma conexão de cliente.
    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) {
            perror("Erro no accept");
            continue;
        }

        cout << "Cliente conectado." << endl;
        
        // Recebendo dados do cliente.
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        while ((bytes_read = read(clientSocket, buffer, BUFFER_SIZE)) > 0) {
            buffer[bytes_read] = '\0';
            cout << "Recebido: " << buffer;
            send(clientSocket, buffer, bytes_read, 0); // Ecoa a mensagem.
        }

        cout << "Conexao encerrada com o cliente." << endl;
        close(clientSocket);
    }

    // Fechando o socket do servidor.
    close(serverSocket);
    return 0;
}