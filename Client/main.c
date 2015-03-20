#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void clean_stdin(void)
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

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
	int sockfd, port, n, cmd,choix, BrewChoix, HowMany;
	char * HowManyMug;
	char * WhenBrew;
	char * WhenChoix;
	char * request;
	char * readSocket;
	struct sockaddr_in serv_addr;
	struct addrinfo *server;
	
	char buffer[1024];
	sockfd = tcp_connect("127.0.0.1","80");
	if (sockfd < 0)
		error("ERROR opening socket");

	while(choix!=9)
		{
			printf("\n##\n");
			printf("## Welcome to the HTCPCP Client\n");
			printf("## This project was created for the C programming lesson of ESGI school.\n");
			printf("## The project is released under the GNU GPL v2.0 License.\n");
			printf("## Author: Dylan Dubief.\n");
			printf("## Last update: 23/02/15\n");
			printf("############################################################################\n");
			printf("\n\n\n");
			printf("1) Brew request\n");
			printf("2) Get request\n");
			printf("3) Propfind request\n");
			printf("9) Quitter le C&C\n");
			printf("\n\nChoix : ");
			scanf(" %d",&choix);
			if((choix<1 && choix !=0) || (choix>3 && choix!=9))
			{
				printf("\nMauvaise commande.\n");
				choix=0;
			}
			else if(choix==1)
			{
				printf("\n\nDo you want your coffee now ?\n");
				printf("1) Yeah, i need coffeeeeeee\n");
				printf("2) No, I'll tell you when.\n");
				scanf(" %d",&BrewChoix);
				if(BrewChoix==1)
				{
					request=calloc(124,sizeof(char));
					readSocket=calloc(512,sizeof(char));
					HowManyMug=calloc(2,sizeof(char));
					printf("\nHow many mugs do you want ?\n");
					clean_stdin();
					scanf("%d",&HowMany);
					if(HowMany>0 && HowMany<=8)
					{
						sprintf(HowManyMug,"%d",HowMany);
						strncpy(request,"BREW /pot-1 HTCPCP/1.1\r\nContent-Type: message/coffeepot\r\nAccept-additions: no-additions\r\nContent-Length: ", 105*sizeof(char));
						strncat(request,HowManyMug,2*sizeof(char));
						printf("Request:\n%s",request);

						n=write(sockfd,request,strlen(request));
						if(n<0)
							error("Error writing socket\n");
						n=read(sockfd,readSocket,512);
						if(n<0)
							error("Error reading socket\n");
						printf("\nResponse: %s\n",readSocket);

					}
					free(HowManyMug);
					free(request);
					//free(readSocket);
				}
				else if(BrewChoix==2)
				{

					request=calloc(124,sizeof(char));
					WhenBrew=calloc(2,sizeof(char));
					HowManyMug=calloc(2,sizeof(char));
					readSocket=calloc(512,sizeof(char));
					printf("\nWhen do you want your coffee ? (Minute)\n");
					clean_stdin();
					scanf("%d",&HowMany);
					if(HowMany>0 && HowMany<121)
					{
						sprintf(WhenBrew,"%d",HowMany);
						strncpy(request,"WHEN /pot-1 HTCPCP/1.1\r\nWhen-I-Want: ",37);
						strncat(request,WhenBrew,strlen(WhenBrew));
						strcat(request,"\r\n");
						printf("\nHow many mugs do you want ?\n");
						clean_stdin();
						scanf("%d",&HowMany);
						if(HowMany>0 && HowMany<=8)
						{	
							sprintf(HowManyMug,"%d",HowMany);
							strncat(request,"Content-Type: message/coffeepot\r\nAccept-additions: no-additions\r\nContent-Length: ", 105*sizeof(char));
							strncat(request,HowManyMug,2*sizeof(char));
							//todo : send
							printf("\nRequest:\n%s",request);
							
							n=write(sockfd,request,strlen(request));
							if(n<0)
								error("Error writing socket\n");
							n=read(sockfd,readSocket,512);
							if(n<0)
								error("Error reading socket\n");
							printf("\nResponse: %s\n",readSocket);

						}
					}
					free(readSocket);
					free(HowManyMug);
					free(request);
					free(WhenBrew);
				}
			}
			else if(choix==2)
			{
				request=calloc(22,sizeof(char));
				readSocket=calloc(512,sizeof(char));
				snprintf(request,22,"GET /pot-1 HTCPCP/1.1");
				n=write(sockfd,request,strlen(request));
				if(n<0)
					error("Error writing socket\n");
				n=read(sockfd,readSocket,512);
				if(n<0)
					error("Error reading socket\n");
				printf("\nResponse: %s\n",readSocket);
				
				free(readSocket);
				free(request);
			}
			else if(choix==3)
			{
				request=calloc(27,sizeof(char));
				readSocket=calloc(512,sizeof(char));
				snprintf(request,27,"PROPFIND /pot-1 HTCPCP/1.1");
				n=write(sockfd,request,strlen(request));
				if(n<0)
					error("Error writing socket\n");
				n=read(sockfd,readSocket,512);
				if(n<0)
					error("Error reading socket\n");
				printf("\nResponse: %s\n",readSocket);
				free(readSocket);
				free(request);
			}
			else if(choix==9)
			{
				close(sockfd);
				exit(1);
			}
			sleep(2);
		}


	return 0;

}
