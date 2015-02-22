#include "lib.h"

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
	fd_set rdfs;
	FILE * fic;
	FILE * ftest;
	struct timeval tv = {1, 0}; 
	int actual = 0;
	int i=0;
	int compt;
	int max = sock;
	Client clients[MAX_CLIENTS];
	

	while(1)
	{



		int i=0;
		FD_ZERO(&rdfs);
		FD_SET(sock, &rdfs);
		// Ajout des sockets de chaque clients.
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
	//char test[10];
	int taill=strlen(buffer);
   	//sprintf(test,"%d",send(csock, buffer, strlen(buffer+1), MSG_DONTWAIT));
   	if(send(csock, buffer, strlen(buffer), MSG_DONTWAIT))
   	{
    	return 0;
   	}
   	else
   	{
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
