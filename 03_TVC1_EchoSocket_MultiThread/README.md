# TVC1

## C++

## echo_client.cpp

Implementação de um cliente TCP que se conecta ao servidor usando IP e portas definidos.  
Permite ao usuário enviar os comandos: `echo <mensagem>` e `quit`.

## echo_server_mt.cpp

Implementação de um servidor TCP que cria uma nova thread para cada novo usuário.  
A cada novo cliente conectado, cria-se uma nova thread.  
A thread roda de forma independente e será destruída automaticamente quando o cliente se desconectar.

## echo_server_pool.cpp

Implementação de um servidor TCP que cria uma fila de sockets dos clientes para serem atendidos pelas threads.  
Usa um pool fixo de 10 threads.  
Se a fila estiver cheia, o servidor responde com uma mensagem de ocupado e fecha a conexão com o cliente.

---

## Python

## echo_client.py

Implementação de um cliente TCP que se conecta ao servidor usando IP e portas definidos.  
Permite ao usuário enviar os comandos: `echo <mensagem>` e `quit`.

## echo_server_mt.py

Implementação de um servidor TCP que cria uma nova thread para cada novo usuário.  
A cada novo cliente conectado, cria-se uma nova thread.  
A thread roda de forma independente e será encerrada automaticamente quando o cliente se desconectar.

## echo_server_pool.py

Implementação de um servidor TCP que cria uma fila de sockets dos clientes para serem atendidos pelas threads.  
Usa um pool fixo de 10 threads.  
Se todas as threads estiverem ocupadas, o servidor responde com uma mensagem de ocupado e fecha a conexão com o cliente.
