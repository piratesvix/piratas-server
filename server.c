#ifdef __unix__         
  #include <unistd.h>
	#include <stdio.h>
	#include <netdb.h>
	#include <netinet/in.h>
	#include <stdlib.h>
	#include <strings.h>
	#include <sys/socket.h>
	#include <sys/types.h>

#elif defined(_WIN32) || defined(WIN32) 
  #define OS_Windows
  #include <io.h>
  #include <winsock2.h>
	#include <ws2tcpip.h>
	#include <iphlpapi.h>
	#include <ws2def.h>
  #include <windows.h>
	#include <unistd.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <strings.h>
	#include <sys/types.h>
	
#endif

#define MAX 80
#define PORT 4200
#define SA struct sockaddr

void func(int connfd)
{
	char buff[MAX];
	int n;
	// loop infinito para bate-papo
	for (;;) {
		bzero(buff, MAX);
		// leia a mensagem que o cliente enviou e copie no buffer
		read(connfd, buff, sizeof(buff));
		// buffer de impressão que contém o conteúdo do cliente
		printf("Cliente: %s\t To client : ", buff);
		bzero(buff, MAX);
		n = 0;
		// copia a mensagem do servidor no buffer
		while ((buff[n++] = getchar()) != '\n')
			;
		// e envia esse buffer para o cliente
		write(connfd, buff, sizeof(buff));
		// se a msg contiver "Sair", a saída do servidor e o bate-papo serão encerrados
		if (strncmp("exit", buff, 4) == 0) {
			printf("Cliente saiu...\n");
			break;
		}
	}
}

int main() {
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	#ifdef OS_Windows
		/* Windows */	
		// criação e verificação de soquete
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd == -1) {
			printf("falha ao tentar criar socket...\n");
			exit(0);
		}
		else
			printf("Iniciando Chat Rum...\n");
		bzero(&servaddr, sizeof(servaddr));

		// especifícar IP, PORT
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr.sin_port = htons(PORT);

		// vinculando o soquete recém-criado ao IP
		if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
			printf("falha ao tentar ligar o socket...\n");
			exit(0);
		}
		else
			printf("\n");

		// verificar funcionamento do servidor com o cliente
		if ((listen(sockfd, 5)) != 0) {
			printf("falha ao tentar ouvir servidor...\n");
			exit(0);
		}
		else
			printf("Escutando..\n");
		len = sizeof(cli);
		// aceite o pacote de dados do cliente
		connfd = accept(sockfd, (SA*)&cli, &len);
		if (connfd < 0) {
			printf("falha ao tentar aceitar pacotes do cliente...\n");
			exit(0);
		}
		else
			printf("conectando ao cliente...\n");
		// função para conversar entre cliente e servidor
		func(connfd);
		// depois de conversar, feche o socket
		close(sockfd);
		
	#else
	  /* GNU/Linux */
		// criação e verificação de soquete
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd == -1) {
			printf("falha ao tentar criar socket...\n");
			exit(0);
		}
		else
			printf("Iniciando Chat Rum...\n");
		bzero(&servaddr, sizeof(servaddr));

		// especifícar IP, PORT
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr.sin_port = htons(PORT);

		// vinculando o soquete recém-criado ao IP
		if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
			printf("falha ao tentar ligar o socket...\n");
			exit(0);
		}
		else
			printf("\n");

		// verificar funcionamento do servidor com o cliente
		if ((listen(sockfd, 5)) != 0) {
			printf("falha ao tentar ouvir servidor...\n");
			exit(0);
		}
		else
			printf("Escutando..\n");
		len = sizeof(cli);
		// aceite o pacote de dados do cliente
		connfd = accept(sockfd, (SA*)&cli, &len);
		if (connfd < 0) {
			printf("falha ao tentar aceitar pacotes do cliente...\n");
			exit(0);
		}
		else
			printf("conectando ao cliente...\n");
		// função para conversar entre cliente e servidor
		func(connfd);
		// depois de conversar, feche o socket
		close(sockfd);
	#endif 
}
