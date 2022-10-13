#include "net.h"


int main(void)
{
	int fd = -1;
	struct sockaddr_in sin;
	if((fd = socket (AF_INET,SOCK_DGRAM, 0))<0)
	{
		perror("socket");
		exit(1);
	}
	
	int b_reuse = 1;
	setsockopt (fd,SOL_SOCKET,SO_REUSEADDR,&b_reuse,sizeof(int ));
	
	bzero(&sin,sizeof(sin));
	sin.sin_family=AF_INET;
	sin.sin_port = htons(SERV_PORT);
	
#if 1
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
#else 
	if(inet_pton(AF_INET ,SERV_IP_ADDR,(void *)&sin.sin_addr)!=1)
	{
		perror("inet_pton");
		exit(1);
	}
#endif
	if(bind(fd,(struct sockaddr *)&sin,sizeof(sin))<0)
	{
		perror("bind");
		exit(1);
	}
	printf("Server starting . . .OK!\n");
	char buf[BUFSIZ];
	struct sockaddr_in cin;
	socklen_t addrlen = sizeof(cin);
	while(1)
	{
		bzero(buf,BUFSIZ);
		if(recvfrom(fd,buf,BUFSIZ-1,0,(struct sockaddr *)&cin,&addrlen)<0)
		{
			perror("recvfrom");
			continue;
		}
		char ipv4_addr[16];
		if(!inet_ntop(AF_INET, (void *)&cin.sin_addr, ipv4_addr,sizeof(cin)))
		{
			perror("inet_ntop");
			exit(1);
		}
		printf("Recive from (%s: %d), data:%s",ipv4_addr,ntohs(sin.sin_port),buf);
		if(!strncasecmp(buf,QUIT_STR, strlen(QUIT_STR)))
		{
			printf("Client(%s:%d) is exiting!\n",ipv4_addr,ntohs(sin.sin_port));

		}
	}
	return 0;
}
