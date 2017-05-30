#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <strings.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/select.h>
#include <unistd.h>

#include <unistd.h>

#include <poll.h> /* poll function */
#include <errno.h>
#include <limits.h>
#define ERR_EXIT(m) \
    do { \
        perror(m); \
        exit(EXIT_FAILURE); \
    } while (0)

#ifndef OPEN_MAX
#define OPEN_MAX 40960
#endif

void do_service(int);
void do_client(int);
void str_read(int);
void display(int);

struct args
{

	long arg1;
	long arg2;
};


struct result
{
	long result;
};


void do_service(int conn)
{
	char recvbuf[1024];
	while(1){
		memset(recvbuf, 0, sizeof(recvbuf));
		str_read(conn);
/*
		int ret = recv(conn, recvbuf, sizeof(recvbuf), 0);
		
		if (ret==0){
			printf("client close\n");
			break;
		}else if(ret==-1){
			ERR_EXIT("read error");
		}

		fputs(recvbuf, stdout);
		write(conn, recvbuf, ret);
	*/
		


	}
/*
	char buf = "Hello GET\n";
	
	printf("start send \n");
	write(fd, buf, sizeof(buf));

	write(stdin, buf, sizeof(buf));
*/
	
}

void do_client(int sockfd)
{
	char buffer[256]; 

	int n;

	printf("Please enter the message: ");

	bzero(buffer,256);

        while(fgets(buffer,255,stdin)) {

		n = write(sockfd,buffer,strlen(buffer));

		if (n < 0)
			error("ERROR writing to socket");
	}

	bzero(buffer,256);

	n = read(sockfd,buffer,255);
	if (n < 0)
		error("ERROR reading from socket");

	printf("%s\n",buffer);
}

void str_read(int fd)
{
	
	fd_set rfds, wfds;
	struct timeval tv;

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);

	FD_SET(fd, &rfds);
	FD_SET(fd, &rfds);

	tv.tv_sec = 10;
	tv.tv_usec = 0;

	int ret;
	int selres = select(fd+1, &rfds, &wfds, NULL, &tv);
	switch(selres)
	{
		case -1:
			printf("select error\n");  
                        ret = -1;  
			break;

		case 0:
			printf("select time out\n");  
                        ret = -1; 
			break;


		default:
			printf("test %d\n", selres);
			if (FD_ISSET(fd, &rfds) || FD_ISSET(fd, &wfds)) {
				display(fd);

			}
			break;

	}
	
}


void display(int fd){
	char buf[1024];

	int bytes = read(fd, buf, 1024);
	printf("bytest %d \n", bytes);
	if (bytes) {
		printf("%s", buf);
	} else {

		printf("not data coming \n");
		exit(-1);
	}

}
