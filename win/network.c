#include <windows.h>
#include <winsock.h>

#include "../main.h"



void networkInit(){
	struct WSAData WSADat; 
	WSAStartup(0x0202, &WSADat); // Инициализация WinSock
							// 0x0202 - версия WinSock. Может быть 1.0, 1.1, 2.0, 2.2
							// WSADat - структура, куда будут занесены рез. инициализации
}

void networkExit(){
	WSACleanup();
}

int networkConn(char * addr,int port){
	struct WSAData WSADat; 
	struct sockaddr_in sin;
	LPHOSTENT hostEntry;
	int sock;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	//store information about the server
	hostEntry = gethostbyname(addr);

	sin.sin_family = AF_INET; // Тип адреса
//	sin.sin_addr.s_addr = inet_addr(IP); // IP-адрес сервера (пори создании сервера можно 0)
	sin.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);
	sin.sin_port = htons(port); // Номер порта сервера

	
	// ***** Подключение к серверу
	printf("Connecting to server...\n");

	if (connect(sock, (struct sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR) {
		printf("Connection error!\n");
		return 0;
	}
	return sock;
}


int networkClose(int sock){
	closesocket(sock);
}

//need network check 

int networkSetNonBlock(int sock){
	u_long l = 1;
	if (SOCKET_ERROR == ioctlsocket (sock, FIONBIO, (unsigned long* ) &l) )
		return -1;
	return 0;
}

int networkSetBlock(int sock){
	u_long l = 0;
	if (SOCKET_ERROR == ioctlsocket (sock, FIONBIO, (unsigned long* ) &l) )
		return -1;
	return 0;
}

int sendData(int socket,void * buf,int size){
	return send(socket,buf,size,0);
}

int recvData(int socket,void * buf,int size){
	return recv(socket,buf,size,0);
}
