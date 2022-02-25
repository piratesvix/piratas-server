#include <iostream>
#include <WS2tcpip.h>
#include <string.h>
#include <sstream>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main() {
	// iniciar winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0) {
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return 0;
	}
	
	// criar um socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		cerr << "Não foi possível criar um socket" << endl;
		return 0;
	}

	// vincular endereço IP e a porta para um socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;
	
	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// preparar socket para escutar
	listen(listening, SOMAXCONN);

	// crie um conjunto de descritores de arquivo master e zere-o
	fd_set master;
	FD_ZERO(&master);

	// adicionar nosso primeiro soquete com o qual estamos interessados ​​em interagir;
	// é importante que este socket seja adicionado para o nosso servidor ou então não 'ouviremos' a entrada de conexões
	FD_SET(listening, &master);

	// condicional para comando \quit
	bool running = true; 

	while (running) {
		// Faça uma cópia do conjunto de descritores de arquivo master, a cópia contém apenas os soquetes que
		// estão aceitando solicitações de conexão de entrada ou mensagens.
		// Por exemplo. Você tem um servidor e seu conjunto de descritores de arquivo master contém 5 itens;
		// o socket escuta quatro clientes. Quando você passa este conjunto para select(),
		// apenas os soquetes que estão interagindo com o servidor são retornados.
		fd_set copy = master;

		// veja quem está falando conosco
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		// faça um loop para todas as conexões atuais
		for (int i = 0; i < socketCount; i++) {
			SOCKET sock = copy.fd_array[i];

			// verifica se é uma comunicação de entrada
			if (sock == listening) {
				// aceitar nova conexão
				SOCKET client = accept(listening, nullptr, nullptr);

				// adicione a nova conexão a lista de clientes conectados
				FD_SET(client, &master);

				// enviar uma mensagem de boas vindas ao cliente conectado
				string welcomeMsg = "Bem vindo ao chat dos Piratas das Ilhas de Vitória!\r\n";
				send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
			} else {
				// mensagem
				char buf[4096];
				ZeroMemory(buf, 4096);
				
				// aceitar mensagem
				int bytesIn = recv(sock, buf, 4096, 0);
				if (bytesIn <= 0) {
					// descartar cliente
					closesocket(sock);
					FD_CLR(sock, &master);
				}
				else {
					// verificar se foi executado comando \quit para dá kill no servidor
					if (buf[0] == '\\')
					{
						// verificar se o comando foi encerrado
						string cmd = string(buf, bytesIn);
						if (cmd == "\\quit")
						{
							running = false;
							break;
						}
						// comando desconhecido
						continue;
					}

					// enviar mensagem para outros clientes
					for (int i = 0; i < master.fd_count; i++)
					{
						SOCKET outSock = master.fd_array[i];
						if (outSock != listening && outSock != sock)
						{
							ostringstream ss;
							ss << "SOCKET #" << sock << ": " << buf << "\r\n";
							string strOut = ss.str();

							send(outSock, strOut.c_str(), strOut.size() + 1, 0);
						}
					}
				}
			}
		}
	}

	// eliminar o socket de escuta do conjunto de descritores e fecha
  // para evitar que mais alguém tente se conectar.
	FD_CLR(listening, &master);
	closesocket(listening);
	
	// mensagem para que os usuários saibam o que está acontecendo
	string msg = "Servidor está sendo desligado. Bye Bye!\r\n";

	while (master.fd_count > 0)
	{
		// obtenha o número do soquete em que o servidor está encerrando
		SOCKET sock = master.fd_array[0];

		// enviando mensagem de adeus para o servidor
		send(sock, msg.c_str(), msg.size() + 1, 0);

		// remover da lista de arquivos master e feche o socket
		FD_CLR(sock, &master);
		closesocket(sock);
	}

	// limpar winsock
	WSACleanup();

	system("pause");

	return 0;
}