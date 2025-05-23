import socket
import threading

SERVER_PORT = 4444
BUFFER_SIZE = 1024

def handle_client(client_socket, address):
    print(f"[{threading.current_thread().name}] Cliente conectado: {address}")
    with client_socket:
        while True:
            data = client_socket.recv(BUFFER_SIZE).decode()
            if not data:
                break

            print(f"[{address}] Recebido: {data.strip()}")
            data = data.strip()

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

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.bind(('', SERVER_PORT))
        server_socket.listen(5)
        print(f"Servidor iniciado na porta {SERVER_PORT}...")

        while True:
            client_socket, address = server_socket.accept()
            thread = threading.Thread(target=handle_client, args=(client_socket, address), daemon=True)
            thread.start()

if __name__ == "__main__":
    main()
