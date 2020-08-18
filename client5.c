#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>
#define PORT 5000
#define MAXLINE 1024
int main()
{
	int sockfd;
	char buffer[MAXLINE];struct sockaddr_in servaddr;
	int n, len;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket creation failed");
		exit(0);
	}
	printf("Socket creation successful\n");
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (connect(sockfd, (struct sockaddr*)&servaddr,sizeof(servaddr)) < 0) {
		printf("\n Error : Connect Failed \n");
	}
	printf("\nWrite here: ");
	fgets(buffer,4096,stdin);
	write(sockfd,buffer,4096);
	printf("\nServerecho:%s",buffer);
	printf("\n");
	close(sockfd);
}
