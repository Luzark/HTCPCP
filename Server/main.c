#include "lib.h"

int main(int argc, char ** argv)
{

	SOCKET sock=init_sock(); // Call to the function "inti_socket ()" to initialize the socket.
	int pid;
	int choix=0;
	
	pid=fork(); // process : network communication.
	
	if(pid==0)
	{
		init_client(sock);
	}
	else
	{
		// process : C&C
		while(choix!=9)
		{
			printf("\n##\n");
			printf("## Welcome to the HTCPCP Server\n");
			printf("## This project was created for the C programming lesson of ESGI school.\n");
			printf("## The project is released under the GNU GPL v2.0 License.\n");
			printf("## Author: Dylan Dubief & Arnaud Zobec.\n");
			printf("## Last update: 22/02/15\n");
			printf("############################################################################\n");
			printf("\n\n\n");
			printf("9) Quitter le C&C\n");
			printf("\n\nChoix : ");
			scanf(" %d",&choix);
			if((choix<1 && choix !=0) || (choix>3 && choix!=9))
			{
				printf("\nMauvaise commande.\n");
				choix=0;
			}
			else if(choix==9)
			{
				kill(pid,SIGKILL);
			}
			sleep(2);
		}
	}

	return 0;
}