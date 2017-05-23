#include "t.h"


int main()
{

	int listenfd, conn;
	int PORT = 8084;

	struct sockaddr_in s_addr, peer_addr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	
	int on = 1;


	if (-1 == setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) {
		printf("failed set sock opt");
		return -1;
	}


	bzero(&s_addr, sizeof(struct sockaddr));
	
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_port = htons(PORT);

	if (-1 == bind(listenfd, (struct sockaddr *)&s_addr, sizeof(struct sockaddr_in))) {
		printf("bind failed \n");
		return -1;

	}

	printf("bind ok\n");


	if (-1 == listen(listenfd, 5)) {
		printf("listen failed\n");
		return -1;

	} else {

		printf("listen ok\n");

	}
	int size, pid;
	while(1) {
		size = sizeof(struct sockaddr_in);
		if (-1 == (conn = accept(listenfd, (struct sockaddr*)&peer_addr, &size))) {

			printf("accept failed \n");

		} else {
			printf("accept ok\n");
		}
		
		pid = fork();
		if (pid == -1)
			ERR_EXIT("fork error");

		if (pid == 0)
		{
			// 子进程
			close(listenfd);
			do_service(conn);
			exit(EXIT_SUCCESS);
		}


		
		close(conn);
	}

	close(listenfd);
}
