#include "net.h"

int main(void)
{
	int fd = -1;
	struct sockaddr_in sin;
	if((fd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("socket");
		exit(1);
	}
	bzero(&sin,sizeof(sin));
	sin.sin_family=AF_INET;
	sin.sin_port = htons(SERV_PORT);
#if 0
	sin.sin_addr.s_addr = inet_addr(SERV_IP_ADDR);
#else
	if(inet_pton(AF_INET,SERV_IP_ADDR,(void *)&sin.sin_addr.s_addr)<0)
	{	
		perror("inet_pton");
		exit(1);
	}
#endif
	if(connect(fd,(struct sockaddr *)&sin,sizeof(sin))<0)
	{
		perror("connect");
		exit(1);
	}
	
	char buf[BUFSIZ];
	while(1)
	{
		bzero(buf,BUFSIZ);
		if(fgets(buf,BUFSIZ-1,stdin)==NULL)
		{
			continue;
		}
		write(fd, buf, strlen(buf));
		if(!strncasecmp(buf,QUIT_STR,strlen(QUIT_STR)))
		{
			printf("client is exting");
			break;
		}
	}	
	
	
}


	
