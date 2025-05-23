#include <iostream>
#include <cstring>          // Para manipulação de strings C.
#include <unistd.h>         // Para close() e read().
#include <sys/socket.h>     // Para socket(), connect().
#include <netinet/in.h>     // Estrutura sockaddr_in.
#include <thread>           // std::thread para multithreading.

#define PORT 4444
#define BUFFER_SIZE 1024

using namespace std;

// Função que será executada por cada thread para lidar com um cliente.
void handleClient(int clientSocket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    cout << "Cliente conectado (Thread: " << this_thread::get_id() << ")" << endl;

    // Loop de comunicação: lê dados do cliente até ele desconectar ou enviar "quit".
    while ((bytes_read = read(clientSocket, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] =  '\0';     // Garante string C terminada.
        string received(buffer);

        cout << "Recebido: " << received;

        // Remove espaços e quebras de linha do final da string.
        received.erase(received.find_last_not_of(" \n\r\t") + 1);

        // Verifica comando recebido.
        if (received.rfind("echo ", 0) == 0) {
            // Se começar com "echo ", envia de volta a mensagem após o comando.
            string response = received.substr(5) + "\n";
            send(clientSocket, response.c_str(), response.length(), 0);
        } else if (received == "quit") {
            // Se receber "quit", encerra a conexão.
            string msg = "Conexao encerrada pelo cliente.\n";
            send(clientSocket, msg.c_str(), msg.length(), 0);
            break;
        } else {
            // Comando inválido.
            string msg = "Comando invalido. Use 'echo <mensagem>' ou 'quit'.\n";
            send(clientSocket, msg.c_str(), msg.length(), 0);
        }
    }

    cout << "Cliente desconectado (Thread: " << this_thread::get_id() << ")" << endl;
    close(clientSocket);    // Fecha socket do cliente.
}

int main() {
    // Criação do socket TCP.
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Erro ao criar o socket");
        return 1;
    }

    // Configuração do endereço do servidor (IP/Porta).
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;     // Aceita conexões de qualquer IP.

    // Associa o socket à porta e IP.
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Erro no bind");
        close(serverSocket);
        return 1;
    }

    // Coloca o socket em modo de escuta.
    if (listen(serverSocket, 5) < 0) {
        perror("Erro no listen");
        close(serverSocket);
        return 1;
    }

    cout << "Servidor multithread iniciado na porta " << PORT << endl;

    // Loop infinito para aceitar conexões de clientes.
    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) {
            perror("Erro no accept");
            continue;   // Tenta aceitar próxima conexão.
        }

        // Cria uma nova thread para cada cliente conectado.
        thread t(handleClient, clientSocket);
        // Destaca a thread para que ela execute independentemente e libere recursos quando acabar.
        t.detach();
    }

    close(serverSocket);    // Boa prática (nunca será alcançado).
    return 0;
}