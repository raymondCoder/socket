#include "t.h"

const int PORT = 8088;


void handle(struct pollfd* clients, int maxClient, int readyClient);

int server_open();

void getC(int);
void getD(int);

int main()
{


	int serverFd;
	printf("start to call server open\n");
	serverFd = server_open();
	printf("start to call getc \n");
	getC(serverFd);
//	getD(serverFd);
	close(serverFd);
	return 0;
}

void getD(int listenfd)
{

	struct sockaddr_in peer;
	int conn;
	char msg[] = "hello\n";
	int len = sizeof(struct sockaddr);
	for(;;){
		if(-1 == (conn=accept(listenfd, (struct sockaddr *)&peer, &len))) {
			printf("failed to get fd \n");
			continue;
		}


		write(conn, msg, sizeof(msg));
		close(conn);
	}

}

void getC(int listenfd)
{

	
	socklen_t socklen = sizeof(struct sockaddr_in);
	struct pollfd clients[OPEN_MAX];
	struct	sockaddr_in cliaddr;
	char buf[1024];
	printf("listen fd = %d\n", listenfd);
	clients[0].fd = listenfd;
	clients[0].events = POLLIN;
	printf("pollin = %d, pollrdnorm = %d\n", POLLIN, POLLRDNORM);

	int i;

	for (i=1; i<OPEN_MAX; i++)
	{
		clients[i].fd = -1;

	}
	

	int maxi = 0;
	int nready;
	int connfd;
	printf("start to loop\n");
	for(;;) {
		printf("client 0 fd %d.   port %d, start tp poll\n", clients[0].fd, PORT);
		/*
		poll(struct pollfd *fdarray, unsigned long nfds, int timeout)
		timeout 1) INFTIME 2) 0) >0)
		*/
		nready = poll(clients, maxi+1, 3000);
		printf("nready = %d \n", nready);
		if (nready==-1) {

			perror("poll error");
		}

		printf("test \n");
		printf("revents %d\n", clients[0].revents);
		if (clients[0].revents & POLLIN) {
			connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &socklen);
			
			sprintf(buf, "accpet from %d:%d\n",  inet_ntoa(cliaddr.sin_addr.s_addr), cliaddr.sin_port);
			printf(buf, "");
			printf("start check clients fd\n");

			for(i=1; i< OPEN_MAX; i++){
				if (clients[i].fd ==-1) {
					clients[i].fd = connfd;
					clients[i].events = POLLIN;
					break;
				}

			}

			if (i==OPEN_MAX) {
			
				fprintf(stderr, "two many connection, more than %d \n", OPEN_MAX);
				close(connfd);
				continue;
			}


			if (i > maxi)
				maxi=i;

			--nready;

		}

		handle(clients, maxi, nready);

	}
	

}

void handle(struct pollfd* clients, int maxClient, int nready) {

	int connfd;
	int i, nread;
	const MAXLINE = 1024;
	char buf[MAXLINE];
	printf("handle is called, nready = %d   maxClient = %d\n", nready, maxClient);
	printf("process id %d \n", getpid());
	if (nready==0)
		return;
	bzero(buf,sizeof(buf));
	for (i=1; i<=maxClient; i++){
		connfd = clients[i].fd;
		printf("client connfd = %d\n",  connfd);
		if (connfd ==-1)
			continue;

		if (clients[i].revents & (POLLIN | POLLERR)) {
			nread = read(connfd, buf, MAXLINE);
			if(nread <0){
				perror("");
				close(connfd);
				clients[i].fd = -1;
				continue;
			}

			if (nread==0) {
				printf("client close the connection");
				close(connfd);
				continue;
			
			}
			printf("=================  [%d]\n", nready);
			printf("buff content = %s [%d]\n", buf, (int)strlen(buf));
			printf("start to send to %d\n", connfd);
			if(strlen(buf) == 2) {
				printf("---------- client want to leave-----------");
			}
			write(connfd, buf, nread);
			if (--nready <=0)
				break;
		
		}
	}
}



int server_open()
{

	int listenFd;
	struct sockaddr_in sockserver;
	if((listenFd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("socket() error!");
		exit(0);
	}
	bzero(&sockserver, sizeof(sockserver));

	sockserver.sin_family = AF_INET;
	sockserver.sin_addr.s_addr = htonl(INADDR_ANY);
	sockserver.sin_port = htons(PORT);
	
	int opt = 1;
	printf("start set opt\n");
	if (setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		perror("setsockopt error");
	}
	printf("start bind\n");
	if (-1 == bind(listenFd, (struct sockaddr *)&sockserver, sizeof(struct sockaddr_in))) {
		error("failed to bind");
	}

	printf("start listen\n");

	if (-1 == listen(listenFd, 5)) {
		error("failed to listen");
	}
	
	return listenFd;
}
