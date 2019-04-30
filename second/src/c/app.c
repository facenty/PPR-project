#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define MAXINPUT 1024
#define MAXOUTPUT 20148
#define FOREVER for(;;)

int main(int argc, char* argv[])
{
	int udpSockFd, udpClientAddressSize, recievedDataSize;
	char inputBuffer[MAXINPUT];
	struct sockaddr_in udpServerAddress, udpClientAddress;

	int tcpSockFd;
	struct sockaddr_in tcpServerAddress, tcpClientAddress;
	char outputBuffer[MAXOUTPUT];
	// if(argc < 4) {
	// 	perror("Wrong number of parameters");
	// 	perror("Usage %s recieveport server_address sendport", argv[0]);
	// 	exit(EXIT_FAILURE);
	// }

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

	FOREVER {
		recievedDataSize = recvfrom(udpSockFd, (char*) inputBuffer, MAXINPUT, MSG_WAITALL, (struct sockaddr*) &udpClientAddress, &udpClientAddressSize);
		inputBuffer[recievedDataSize] = '\0';
		printf("%s", inputBuffer);
		memset(&inputBuffer, 0, MAXINPUT);
	}

	close(udpSockFd);

	return 0;
}