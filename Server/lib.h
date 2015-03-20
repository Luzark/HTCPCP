#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netdb.h> //hostent
#include <sys/select.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <termios.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
#define CRLF		"\r\n"
#define PORT	 	80
#define MAX_CLIENTS 	100
#define BUF_SIZE	1024
#define CLIENT_H

typedef struct
{
   SOCKET sock;
   char name[BUF_SIZE];
}Client;

int init_sock(void);
int read_client(SOCKET sock,  char *buffer);
int init_client(SOCKET sock);
void remove_client(Client *clients, int to_remove, int *actual);
int write_client(SOCKET csock, char *buffer);
int serialport_read_until(int fd, char* buf, char until);
int serialport_init();
int serialport_write(int fd, const char* str);
int communicate(int fd,int comm1, int comm2);