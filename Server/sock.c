#include "lib.h"

int saveifile(char * namefic, char * buffer)
{
	FILE * fic;
	fic=fopen(namefic,"a+");
	if(fic==NULL)
	{
		return 1;
	}


	// A formater
	if(fprintf(fic, "%s\n",buffer ))
		fclose(fic);
	else
		return 2;
	
	return 0;
}

int init_sock(void)
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET)
	{
	    perror("socket()");
	    exit(errno);
	}
	SOCKADDR_IN sin = { 0 };

	sin.sin_addr.s_addr = htonl(INADDR_ANY); 
	sin.sin_family = AF_INET;
	sin.sin_port = htons(80);
	
	if(bind (sock, (SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
	{
	    perror("bind()");
	    exit(errno);
	}
	if(listen(sock,MAX_CLIENTS) == SOCKET_ERROR)
	{
  		perror("listen()");
  		exit(errno);
  	}

	return sock;

}


int init_client(SOCKET sock)
{
	char buffer[BUF_SIZE];
	char * BrewResponse;
	char debug[3];
	fd_set rdfs;
	FILE * fic;
	FILE * ftest;
	struct timeval tv = {1, 0}; 
	int actual = 0;
	int i=0,booleantest=0;
	int compt;
	int rc;
	int commande[2];
	int max = sock;
	Client clients[MAX_CLIENTS];
	
	int fdarduino = 0;
    fdarduino=serialport_init();

	while(1)
	{



		int i=0;
		FD_ZERO(&rdfs);
		FD_SET(sock, &rdfs);
		// Ajout les sockets de chaque clients.
		for(i = 0; i < actual; i++)
		{
		    FD_SET(clients[i].sock, &rdfs); 
		}
		if(select(max + 1, &rdfs, NULL, NULL, &tv) == -1) // Attend l'arrivé d'un client.
	    {
	        perror("select()");
	        exit(errno);
	    }


	    if(FD_ISSET(sock, &rdfs))
	    {
	    	// nouveau client
	    	SOCKADDR_IN csin = { 0 };
	        socklen_t sinsize = sizeof csin;
	        SOCKET csock;
	        csock=accept(sock, (SOCKADDR *)&csin, &sinsize);
	        if(csock != SOCKET_ERROR)
	    	    perror("csock");
	    	if(read_client(csock, buffer) == -1)
            	continue;

            max = csock > max ? csock : max; // on modifie le nombre de client.

            FD_SET(csock, &rdfs);

            Client c = { csock };
         	strncpy(c.name, buffer, BUF_SIZE - 1); // On enregistre le nom du client.
         	clients[actual] = c;
         	actual++;
	    }
	    else
	    {
	    	
	    	for(i = 0; i < actual; i++)
         	{
         		// on cherche les clients en train de communiquer
	            if(FD_ISSET(clients[i].sock, &rdfs)) // Un client communique.
	            {
	               	Client client = clients[i];
	                int c = read_client(clients[i].sock, buffer); // lecture de la communication du client.
	                /* client disconnected */
	                if(c == 0)
	                {
	                	closesocket(clients[i].sock);
                  		remove_client(clients, i, &actual);
	                }
	                else
	                {
	                	if(buffer[0]=='B')
	                	{
	                		if(strncmp(buffer,"BREW",4)==0)
	                		{
								commande[0]=buffer[strlen(buffer)-1]-'0';
	                			if(commande[0]>0 && commande[0]<=8)
	                			{

	                				rc=communicate(fdarduino,commande[0],0);
	                				if(rc==0)
	                				{
		                				BrewResponse=calloc(74, sizeof(char));
		                				snprintf(BrewResponse,74,"HTCPCP/1.1 200 OK\r\nExpires: When the coffee is cold...\r\nContent-Length: %d",commande[0]);	                				
		                				write_client(clients[i].sock,BrewResponse);
	                				}
	                				else
	                				{
	                					BrewResponse=calloc(31,sizeof(char));
	                					snprintf(BrewResponse,31,"HTCPCP/1.1 418 I'm a teapot!\r\n");
	                					write_client(clients[i].sock,BrewResponse);
	                				}
	                			}					
	                		}
	                	}
	                	else if(buffer[0]=='G')
	                	{
	                		if(strncmp(buffer,"GET",3)==0)
	                		{
	                			BrewResponse=calloc(84,sizeof(char));
	                			snprintf(BrewResponse,84,"HTCPCP/1.1 200 OK\r\nServer: HTCPCP\r\nVoltage: 220v\r\nBrand: Selecline\r\nCapacity: 8mugs");
	                			saveifile("log_reponse.txt",BrewResponse);
	                			write_client(clients[i].sock,BrewResponse);
	                		}
	                	}
	                	else if(buffer[0]=='P')
	                	{
	                		if(strncmp(buffer,"PROPFIND",8)==0)
	                		{
	                			BrewResponse=calloc(95,sizeof(char));
	                			snprintf(BrewResponse,95,"HTCPCP/1.1 200 OK\r\nTitle: Cafe Rare\r\nBrand: Paul Dequidt\r\nCountry: Brasil\r\nExpires: 26/04/2014");
	                			saveifile("log_reponse.txt",BrewResponse);
	                			write_client(clients[i].sock,BrewResponse);
	                		}
	                	}
	                	else if(buffer[0]=='W')
	                	{
	                		if(strncmp(buffer,"WHEN",4)==0)
	                		{
	                			if(buffer[39]!='\r' && buffer[39]!='\n')
	                			{
	                				commande[1]=(buffer[37]-'0')*100+(buffer[38]-'0')*10+(buffer[39]-'0');
	                				commande[0]=buffer[strlen(buffer)-3]-'0';
	                			}
	                			else if(buffer[38]!='\r' && buffer[38]!='\n')
	                			{
	                				commande[1]=(buffer[37]-'0')*10+(buffer[38]-'0');
	                				commande[0]=buffer[strlen(buffer)-4]-'0';
	                			}
	                			else
	                			{
	                				commande[1]=(buffer[37]-'0');
	                				commande[0]=buffer[strlen(buffer)-5]-'0';
	                			}
	                			rc=communicate(fdarduino,commande[0],commande[1]);
	                			if(rc==0)
	                			{
	                				BrewResponse=calloc(90, sizeof(char));
	                				snprintf(BrewResponse,90,"HTCPCP/1.1 200 OK\r\nExpires: When the coffee is cold...\r\nReady: %dmins\r\nContent-Length: %d",commande[1],commande[0]);
	                				write_client(clients[i].sock,BrewResponse);
	                			}
	                			else
	                			{
	                				BrewResponse=calloc(33,sizeof(char));
	                				snprintf(BrewResponse,33,"HTCPCP/1.1 406 Not acceptable.\r\n");
	                				write_client(clients[i].sock,BrewResponse);
	                			}
	                		}
	                	}
	                	free(BrewResponse);
	                }
	            
	            }
         	}
	    }
	}

	return 0;
}

//Lecture des données envoyées par un client.
int read_client(SOCKET csock,  char *buffer)
{
   	int n = 0;

	if((n = recv(csock, buffer, 1024, MSG_DONTWAIT)) < 0)
	{
		perror("recvfrom()");
		return 0;
	}

	// TODO : Retourner la commande du client.

   	return n;
}

int write_client(SOCKET csock, char *buffer)
{
	int taill=strlen(buffer);
   	if(send(csock, buffer, strlen(buffer), MSG_DONTWAIT))
   	{
    	return 0;
   	}
   	else
   	{
   		printf("\nWTF\n");
   		perror("send()");
   		return -1;
   	}
}

// Suppression d'un client dans la tab de socket
void remove_client(Client *clients, int to_remove, int *actual)
{
   
   memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove - 1) * sizeof(Client));
   
   (*actual)--;
}
