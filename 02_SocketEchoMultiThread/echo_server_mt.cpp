#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 4444
#define BUFFER_SIZE 1024
#define THREAD_POOL_SIZE 5

using namespace std;

// Fila de sockets dos clientes.
queue<int> clientQueue;
mutex queueMutex;
condition_variable queueCond;

// Função que lida com clientes em thread separada.
void handleClient() {
    while (true) {
        int clientSocket;

        // Aguardando uma conexão na fila.
        {
            unique_lock<mutex> lock(queueMutex);
            queueCond.wait(lock, []() { return !clientQueue.empty(); });

            clientSocket = clientQueue.front();
            clientQueue.pop();
        }

        // Processando a conexão.
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;

        cout << "[Thread " << this_thread::get_id() << "] Cliente conectado." << endl;

        while ((bytes_read = read(clientSocket, buffer, BUFFER_SIZE - 1)) > 0) {
            buffer[bytes_read] = '\0';
            string received(buffer);

            received.erase(received.find_last_not_of(" \n\r\t") + 1);

            cout << "[Thread " << this_thread::get_id() << "] Recebido: " << received << endl;

            if (received.rfind("echo ", 0) == 0) {
                string response = received.substr(5) + "\n";
                send(clientSocket, response.c_str(), response.length(), 0);
            } else if (received == "quit") {
                string msg = "Conexao encerrada pelo cliente.\n";
                send(clientSocket, msg.c_str(), msg.length(), 0);
                break;
            } else {
                string msg = "Comando invalido. Use 'echo <mensagem>' ou 'quit'.\n";
                send(clientSocket, msg.c_str(), msg.length(), 0);
            }
        }

        cout << "[Thread " << this_thread::get_id() << "] Cliente desconectado." << endl;
        close(clientSocket);
    }
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Erro ao criar o socket");
        return 1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Erro no bind");
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 5) < 0) {
        perror("Erro no listen");
        close(serverSocket);
        return 1;
    }

    cout << "Servidor multithread iniciado na porta " << PORT << endl;

    // Criando o pool de threads.
    vector<thread> threadPool;
    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        threadPool.emplace_back(handleClient);
    }

    // Aceitar conexões e adicionar à file de threads.
    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) {
            perror("Erro no accept");
            continue;
        }

        // Lidar com o cliente.
        {
            lock_guard<mutex> lock(queueMutex);
            clientQueue.push(clientSocket);
        }
        queueCond.notify_one(); // Acorda uma thread do pool.
    }

    close(serverSocket);
    return 0;
}