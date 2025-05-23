import socket
import threading
import queue

SERVER_PORT = 4444
BUFFER_SIZE = 1024
THREAD_POOL_SIZE = 10

client_queue = queue.Queue(maxsize=THREAD_POOL_SIZE)

def handle_clients():
    while True:
        client_socket, address = client_queue.get()
        print(f"[{threading.current_thread().name}] Cliente conectado: {address}")
        with client_socket:
            while True:
                data = client_socket.recv(BUFFER_SIZE).decode()
                if not data:
                    break

                data = data.strip()
                print(f"[{address}] Recebido: {data}")

                if data.startswith("echo "):
                    response = data[5:] + "\n"
                elif data == "quit":
                    response = "Conexao encerrada pelo cliente.\n"
                    client_socket.sendall(response.encode())
                    break
                else:
                    response = "Comando invalido. Use 'echo <mensagem>' ou 'quit'.\n"

                client_socket.sendall(response.encode())

        print(f"[{threading.current_thread().name}] Cliente desconectado: {address}")
        client_queue.task_done()

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.bind(('', SERVER_PORT))
        server_socket.listen(5)
        print(f"Servidor com thread pool iniciado na porta {SERVER_PORT}")

        # Cria as threads do pool
        for i in range(THREAD_POOL_SIZE):
            thread = threading.Thread(target=handle_clients, daemon=True)
            thread.start()

        while True:
            client_socket, address = server_socket.accept()

            if client_queue.full():
                client_socket.sendall(b"Servidor ocupado. Tente novamente mais tarde.\n")
                client_socket.close()
            else:
                client_queue.put((client_socket, address))

if __name__ == "__main__":
    main()
