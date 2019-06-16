#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <netdb.h> 

#define MAXINPUT 1024
#define MAXOUTPUT 2048
#define FOREVER for(;;)

void string2hexString(char* input, char* output, int max)
{
	int loop;
	int i; 

	i=0;
	loop=0;

	for (loop; loop < max; ++loop)
	{
		sprintf((char*)(output+i),"%02X", input[loop]);
		i+=2;
	}
	output[i++] = '\0';
}

int main(int argc, char* argv[])
{
	int udpSockFd, udpClientAddressSize, recievedDataSize;
	char inputBuffer[MAXINPUT];
	struct sockaddr_in udpClientAddress, udpServerAddress;

	int tcpSockFd;
	char outputBuffer[MAXOUTPUT];
	struct sockaddr_in tcpServerAddress;

	if(argc < 4) {
		perror("Wrong number of parameters");
		printf("Usage %s recieveport server_address sendport", argv[0]);
		exit(EXIT_FAILURE);
	}

	memset(&inputBuffer, 0, MAXINPUT);
	memset(&outputBuffer, 0, MAXOUTPUT);
	memset(&udpServerAddress, 0, sizeof(udpServerAddress));
	memset(&udpClientAddress, 0, sizeof(udpClientAddress));

	if ((udpSockFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Udp socket creation failed");
		exit(EXIT_FAILURE);
	}

	udpServerAddress.sin_family = AF_INET;
	udpServerAddress.sin_addr.s_addr = INADDR_ANY;
	udpServerAddress.sin_port = htons(atoi(argv[1]));

	if ( bind(udpSockFd, (const struct sockaddr *)&udpServerAddress, sizeof(udpServerAddress)) < 0 ) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if ((tcpSockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Udp socket creation failed");
		exit(EXIT_FAILURE);
	}

	tcpServerAddress.sin_family = AF_INET;
	tcpServerAddress.sin_port = htons(atoi(argv[3]));
	struct hostent *server = gethostbyname(argv[2]);
	bcopy( (char *) server->h_addr, (char *) &tcpServerAddress.sin_addr.s_addr, server->h_length );

	if( connect( tcpSockFd, (struct sockaddr *) &tcpServerAddress, sizeof( tcpServerAddress ) ) < 0 ) {
		perror("connect failed");
		exit(EXIT_FAILURE);
	}
	else {
		printf("Connected to server \n");
	}

	FOREVER {
		recievedDataSize = recvfrom(udpSockFd, (char*) inputBuffer, MAXINPUT-1, MSG_WAITALL, (struct sockaddr*) &udpClientAddress, &udpClientAddressSize);
		inputBuffer[recievedDataSize] = '\0';
		printf("%s", inputBuffer);
		int i,j;
		for(i=0,j=0;i<recievedDataSize;i++,j+=2)
		{
			sprintf((char*)outputBuffer+j,"%02X",inputBuffer[i]);
		}
		// printf("====> %s \n", outputBuffer);
		outputBuffer[2*recievedDataSize] = '\0';
		printf("Len of outb: %d \n", strlen(outputBuffer));
		if (send(tcpSockFd, outputBuffer, MAXOUTPUT, 0) < 0) {
			perror("Send failed");
			exit(EXIT_FAILURE);
		}
		memset(&inputBuffer, 0, MAXINPUT);
		memset(&outputBuffer, 0, MAXOUTPUT);
	}

	close(udpSockFd);
	close(tcpSockFd);

	return 0;
}