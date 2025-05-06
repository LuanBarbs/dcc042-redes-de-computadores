import socket;

HOST = '0.0.0.0' # Executa todas as interfaces.
PORT = 4444

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
    server_socket.bind((HOST, PORT))
    server_socket.listen()

    print(f"Servidor iniciado na porta {PORT}")

    while True:
        client_socket, addr = server_socket.accept()
        print(f"Conexão aceita de {addr}")

        with client_socket:
            while True:
                data = client_socket.recv(1024)
                if not data:
                    break
                print(f"Recebido: {data.decode()}")
                client_socket.sendall(data)
        
        print("Conexão encerrada")