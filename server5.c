#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define PORT 5000
#define MAXLINE 1024
int max(int x, int y)
{
	if (x > y)
		return x;
	else
	return y;
}

int main()
{
	int listenfd, connfd, udpfd, nready, maxfdp1;
	char buffer[MAXLINE];
	pid_t childpid;fd_set rset;
	ssize_t n;
	socklen_t len;
	const int on = 1;
	struct sockaddr_in cliaddr, servaddr;
	char *message = "Hello Client.";
	void sig_chld(int);
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd<0)
		{ printf("error in socket creation.\n"); }
	printf("TCP Socket created successfully.\n");
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	// binding server
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	listen(listenfd, 5);
	printf("Listening...\n");
	udpfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(udpfd<0)
		{ printf("error in socket creation.\n"); }
	printf("UDP Socket created successfully.\n");
	// binding server
	bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	// clear the descriptor set
	FD_ZERO(&rset);
	// get maxfd
	maxfdp1 = max(listenfd, udpfd) + 1;
	for (;;) {
	// set listenfd and udpfd in readset
		FD_SET(listenfd, &rset);
		FD_SET(udpfd, &rset);
	// select the ready descriptor
		nready = select(maxfdp1, &rset, NULL, NULL, NULL);// if tcp socket is readable then handle
	// it by accepting the connection
		if (FD_ISSET(listenfd, &rset)) {
			len = sizeof(cliaddr);
			connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);
			if ((childpid = fork()) == 0) {
				close(listenfd);
				read(connfd,buffer,1024);
				printf("\nClient message:%s",buffer);
				write(connfd,buffer,1024);
			}
			close(connfd);
		}
	// if udp socket is readable receive the message.
		if (FD_ISSET(udpfd, &rset)) {
			len = sizeof(cliaddr);
			bzero(buffer, sizeof(buffer));
			printf("\nMessage from UDP client: ");
			n = recvfrom(udpfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr, &len);
			puts(buffer);
			sendto(udpfd, (const char*)message, sizeof(buffer), 0, (struct sockaddr*)&cliaddr,
			sizeof(cliaddr));
		}
	}
}
