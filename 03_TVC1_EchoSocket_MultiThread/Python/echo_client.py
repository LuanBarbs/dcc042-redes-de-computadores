import socket
import sys

def main():
    if len(sys.argv) < 2:
        print(f"Uso: python {sys.argv[0]} <IP> [PORTA]")
        return

    host = sys.argv[1]
    port = int(sys.argv[2]) if len(sys.argv) >= 3 else 4444

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        print(f"Conectado ao servidor {host}:{port}")
        print("Use os comandos:\n - echo <mensagem>\n - quit")

        while True:
            msg = input("> ")
            if not msg:
                continue
            s.sendall(msg.encode())

            if msg.strip() == "quit":
                break

            resposta = s.recv(1024).decode()
            print("Resposta:", resposta.strip())

if __name__ == "__main__":
    main()
