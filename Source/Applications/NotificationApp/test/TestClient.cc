/*
 * TestClient.cc
 *
 *  Created on: 11-Nov-2017
 *      Author: div
 */


// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 50013

int main(int argc, char const *argv[])
{
	struct sockaddr_in address;
	int sock = 0, valread;

	struct sockaddr_in serv_addr;
	char hello1[] = "14 nov Childrens day remind yearly";
	char hello[] = "5 aug Clean Guest Room remind yearly";
	char buffer[1024] = {0};

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	printf("Client Socket created\n");

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");


	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	printf("Client connected succesfully\n");
	int returnCode= send(sock , hello1 , sizeof(hello1) , 0 );
	if ( -1 == returnCode)
	{
		printf("Sent error\n");
	}
	//returnCode = send(sock , hello , sizeof(hello) , 0 );
	if ( -1 == returnCode)
	{
		printf("Sent error\n");
	}
	printf("Hello message sent\n");
	//valread = read( sock , buffer, 1024);
	//printf("%s\n",buffer );

	close (sock);
	shutdown(sock,SHUT_RDWR);

	return 0;
}

