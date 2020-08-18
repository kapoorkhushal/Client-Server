#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 22000
int main()
{
	int server_fd, new_socket, valread;
	struct sockaddr_in servaddr;
	char str[100];
	int addrlen = sizeof(servaddr);
	char buffer[1024] = { 0 };
	if ((server_fd = socket(AF_INET,SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	printf("Socket created successfully.\n");
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	if (bind(server_fd, (struct sockaddr*)&servaddr,sizeof(servaddr)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("Listening...\n");
	if ((new_socket = accept(server_fd,(struct sockaddr*)&servaddr, (socklen_t*)&addrlen)) <0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	valread = read(new_socket, str, sizeof(str));
	int i, j, temp;
	int l = strlen(str);
	printf("\nString sent by client:%s\n", str);
	// loop to reverse the string
	for (i = 0, j = l - 1; i < j; i++, j--) {
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
	}// send reversed string to client
	// by send system call
	send(new_socket, str, sizeof(str), 0);
	printf("\nModified string sent to client\n");
	return 0;
}
