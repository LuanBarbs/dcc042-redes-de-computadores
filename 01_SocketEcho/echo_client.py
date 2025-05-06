import socket
import sys

if len(sys.argv) < 3:
    print("Uso: python echo_client.py <seu_nome> <host>")
    sys.exit(1)

screen_name = sys.argv[1]
host = sys.argv[2]
PORT = 4444

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((host, PORT))
    print(f"Conectado ao servidor {host}:{PORT}")

    while True:
        msg = input("Digite sua mensagem (ou 'exit' para sair): ")
        if msg.strip().lower() == 'exit':
            break

        full_msg = f"[{screen_name}]: {msg}"
        s.sendall(full_msg.encode())

        data = s.recv(1024)
        print(f"Eco do servidor: {data.decode()}")

    print("Conexão encerrada.")