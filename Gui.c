#ifdef _WIN32
	#include <winsock2.h> // socket, connect
#else
	#include <sys/socket.h> // socket, connect
	#include <netdb.h>
	#include <arpa/inet.h> // inet_addr
	#include <unistd.h>    // read, write, close
#endif

#include <stdio.h>     // printf, sprintf, 
#include <string.h>    // memset, memcpy, strlen, strchr
#include <stdlib.h>    // atoi
#include "Gui.h"

#define PORT 8080
#define BUF_SIZE 1024

static char inBuf[BUF_SIZE];
static char outBuf[BUF_SIZE];
static int sock = -1;
static struct sockaddr_in serv_addr;

static int isConnected = 0;



void guiConnect(void)
{
	if (isConnected) {
		guiDisconnect();
	}

#ifdef _WIN32
	// http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html
	// http://www.c-worker.ch/tuts/wstut_op.php
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2,0), &wsaData) != 0) {
		printf("WSAStartup failed.\n");
		exit(1);
	}
#endif

	// create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		printf("open stream socket");
		exit(1);
	}

	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// open connection
	if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0) {
		printf("cannot connect to GUI\n");
		printf("start GUI first:\n");
		printf("java -jar spreadsheet-gui.jar\n");
		exit(1);
	}

	printf("connected to GUI\n");
	isConnected = 1;
}



void guiDisconnect(void)
{
	if (sock >= 0) {
#ifdef _WIN32
		closesocket(sock);
#else
		close(sock);
#endif
		sock = -1;
	}
	isConnected = 0;
}



void guiSend(char *input)
{
	if (!isConnected) {
		guiConnect();
	}
#ifdef _WIN32
	if (send(sock, input, strlen(input), 0) < 0) {
#else
	if (write(sock, input, strlen(input)) < 0) {
#endif
		printf("writing to socket");
		exit(1);
	}
}



char* guiReceive()
{
	if (!isConnected) {
		guiConnect();
	}
#ifdef _WIN32
	int n = recv(sock, inBuf, BUF_SIZE, 0);
#else
	int n = read(sock, inBuf, BUF_SIZE);
#endif
	if (n < 0) {
		printf("error reading from socket");
		exit(1);
	}
	// printf("guiReceive: %d bytes received\n", n);
	inBuf[n] = '\0';
	return inBuf;
}



void guiSetInput(const char *input, int row, int column)
{
	sprintf(outBuf, "(set-input %d %d \"%s\")\n", row, column, input);
	// printf("%s", outBuf);
	guiSend(outBuf);
}



void guiSetOutput(const char *output, int row, int column)
{
	sprintf(outBuf, "(set-output %d %d \"%s\")\n", row, column, output);
	// printf("%s", outBuf);
	guiSend(outBuf);
}



GuiInput guiGetInput()
{
	GuiInput result;
	memset(&result, 0, sizeof(GuiInput));
	char *s = guiReceive();
	if (s != NULL) {
		// printf("guiGetInput: %s", s); // 123 234 hello
		int row = 0, col = 0;
		char *value = NULL;
		char *p = strchr(s, ' '); // 123p234 hello
		if (p != NULL) {
			row = atoi(s);
			s = p + 1;
			p = strchr(s, ' '); // 123 234phello
			if (p != NULL) {
				col = atoi(s);
				value = p + 1;
				int n = strlen(value);
				//printf("%d %s\n", n, value);
				if (value[n-1] == '\n' || value[n-1] == '\r') {
					value[n-1] = '\0';
				}
				if (value[n-2] == '\n' || value[n-2] == '\r') {
					value[n-2] = '\0';
				}
				if (row >= 0 && col >= 0) {
					result.row = row;
					result.column = col;
					result.value = value;
				}
			}
		}
	}
	return result;
}


