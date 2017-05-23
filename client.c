#include "t.h"


int main()
{

	int fd;
	int ret;
	struct sockaddr_in c_addr;



	fd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&c_addr, sizeof(struct sockaddr_in));


	c_addr.sin_family = AF_INET;
	c_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	c_addr.sin_port = htons(8084);
	
	if (0 == connect(fd, (struct sockaddr *)&c_addr, sizeof(struct sockaddr_in))) {
		printf("socket connect succeed immediately.\n");  
		ret = 0; 

	} else {
		printf("get the connect result by select().\n");  	
		
	}

	do_client(fd);

	close(fd);

	return -1;
}
