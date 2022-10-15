#include "net.h"
#include <signal.h>
#include <pthread.h>
void cli_data_handle(void *arg);
void sig_child_handle(int signo)
{
	if(SIGCHLD == signo)
	{
		waitpid(-1,NULL,WNOHANG);	
	}
		
}

int main(void)
{
	int fd = -1;
	struct sockaddr_in sin;

	signal(SIGCHLD, sig_child_handle);
	if((fd = socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("socket");
		return 0;
	}
	bzero(&sin,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port=htons(SERV_PORT);
#if 1
	sin.sin_addr.s_addr =htonl(INADDR_ANY);
#else
	if(inet_pton(AF_INET,SERV_IP_ADDR,(void *)&sin.sin_addr)!=1)
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
	if(listen(fd,BACKLOG)<0)
	{
		perror("listen");
		exit(1);
	}
	printf("Server starting...OK!\n");
	int newfd =-1;
	struct sockaddr_in cin;
	socklen_t addrlen= sizeof(cin);

	while(1)
	{
		pid_t pid = -1;
		if((newfd = accept(fd,(struct sockaddr *)&cin,&addrlen))<0)
		{
			perror("accept");
			break;
		}
		if((pid = fork())<0)
		{
			perror("fork");
			break;
		}
		if(0==pid)
		{
			close(fd);
			char ipv4_addr[16];
			if(!inet_ntop(AF_INET,(void *)&cin.sin_addr,ipv4_addr,sizeof(cin)))
			{
				perror("net_ntop");
				exit(1);
			}

			printf("Clinet (%s :%d)is connected!",ipv4_addr,ntohs(cin.sin_port));
			cli_data_handle(&newfd);
			return 0;	
			

		}
		else
		{
			close(newfd);
		}
	}

	close(fd);
	return 0;	
}


void cli_data_handle(void *arg)
{	
	int newfd =*(int *) arg;
	printf("Child handling process:newfd =%d\n",newfd);
	int ret =-1;
	char buf[BUFSIZ];
	char resp_buf[BUFSIZ+10];
	while(1)
	{
		bzero(buf,BUFSIZ);
		do{

			ret = read(newfd,buf,BUFSIZ-1);
		}while(ret < 0&&EINTR == errno);
		if(ret<0)
		{
			perror("read");
			exit(1);
		}
		if(!ret)
		{	
			break;
		}
		bzero(resp_buf,BUFSIZ+10);
		strncpy(resp_buf,SERV_RESP_STR,strlen(SERV_RESP_STR));
		printf("Receive data:%s\n",buf);

		strcat(resp_buf,buf);
		do{
			ret = write(newfd,resp_buf,strlen(resp_buf));
		}while(ret < 0&&EINTR == errno);
	

		if(!strncasecmp(buf,QUIT_STR,strlen(QUIT_STR)))
		{
			printf("Client(fd = %d) is exiting!\n",newfd);
			break;
		}
	}
	close(newfd);
}

