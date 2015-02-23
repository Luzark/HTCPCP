#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int tcp_connect(char * address, char * port)
{
	int status, sockfd;
	struct addrinfo hints, *servinfo, *p;

	memset(&hints,0,sizeof(hints));
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype=SOCK_STREAM;
	status=getaddrinfo(address,port,&hints,&servinfo);
	if(status!=0)
	{
		printf("\ntcpconnect: getaddrinfo failed");
		freeaddrinfo(servinfo);
		return -1;
	}
	for(p=servinfo; p!=NULL;p=p->ai_next)
	{
		sockfd=socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if(sockfd==-1)
		{
			printf("\ntcpconnect: socket failed");
			continue;
		}

		status= connect(sockfd, p->ai_addr, p->ai_addrlen);
		if(status==-1)
		{
			close(sockfd);
			printf("\ntcpconnect: connect failed");
			continue;
		}

		break;
	}

	if(p==NULL)
	{
		printf("\ntcpconnect: could not get a valid socket file descriptof");
		freeaddrinfo(servinfo);
		return -1;
	}

	freeaddrinfo(servinfo);

	return sockfd;

}

int main(int argc, char *argv[])
{
	int sockfd, port, n, cmd;
	struct sockaddr_in serv_addr;
	struct addrinfo *server;
	
	char buffer[1024];
	sockfd = tcp_connect("127.0.0.1","80");
	if (sockfd < 0)
		error("ERROR opening socket");

	return 0;

}