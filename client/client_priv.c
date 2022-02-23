#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 80
#define PORT 4200
#define SA struct sockaddr

void func(int sockfd) {
	char buff[MAX];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		printf("Digite: ");
		n = 0;
		while ((buff[n++] = getchar()) != '\n')
			;
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("Server: %s", buff);
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Cliente saiu...\n");
			break;
		}
	}
}

int main() {
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// criar e verificar socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("falha ao tentar iniciar chat...\n");
		exit(0);
	}
	else
		printf("\n");
	// apagar bytes na memória
	bzero(&servaddr, sizeof(servaddr));

	// atribuir IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// conectar o socket do cliente ao socket do servidor
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("falha ao tentar se conectar com servidor...\n");
		exit(0);
	}
	else
		printf("conectado ao servidor..\n");

	// implementar função para o chat
	func(sockfd);

	// adicionar método para fechar o socket
	close(sockfd);
}
