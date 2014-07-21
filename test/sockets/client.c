#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
char Message[200];

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "mswsock.lib")

int main(void) {
	char PCName[30], ServerName[30],  IP[16] = { 0 };
	struct WSAData WSADat; // �������� WinSock (��������� ������� WSAStartup)
	struct sockaddr_in sin; // ��������(�����) ������������ ������
	SOCKET Sock; // ���������� �����
  
	// ���� IP-������ �������
	printf("Enter server's IP: ");
	scanf("%s", IP);
//init	
	WSAStartup(0x0202, &WSADat); // ������������� WinSock
								 // 0x0202 - ������ WinSock. ����� ���� 1.0, 1.1, 2.0, 2.2
								 // WSADat - ���������, ���� ����� �������� ���. �������������
	Sock = socket(AF_INET, SOCK_STREAM, 0); // �������� ������
////	
	gethostname(PCName, 30); // ��������� ����� �������� ��
//connect	
	LPHOSTENT hostEntry;
	//store information about the server
	hostEntry = gethostbyname(IP);

	sin.sin_family = AF_INET; // ��� ������
//	sin.sin_addr.s_addr = inet_addr(IP); // IP-����� ������� (���� �������� ������� ����� 0)
	sin.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);
	sin.sin_port = htons(2803); // ����� ����� �������

	
	// ***** ����������� � �������
	printf("Connecting to server...\n");

	if (connect(Sock, (struct sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR) {
		printf("Connection error!\n");
		goto End;
	}
	
/////
	send(Sock, PCName, strlen(PCName) + 1, 0); // �������� ����� ����� ���������� (�������)
	recv(Sock, ServerName, 30, 0); // ��������� ����� ���������� �������

	printf("Connected to \"%s\"!\n", ServerName);

	FD_SET ReadSet;	
//	FD_SET WriteSet;
	u_long l = 1;
	if (SOCKET_ERROR == ioctlsocket (Sock, FIONBIO, (unsigned long* ) &l) ){
	// Error
return -1;
	}
	
//        FD_ZERO(&WriteSet);
	
	char buf[100];
	// ***** ����
	do {
		Sleep(300);
		int n;
		memset(buf,0,sizeof(buf));
			if ((n=recv(Sock,buf,2,0))<0)
				printf("error %d\n",n);
			else
				printf("%s %d\n",buf,n);
		   
		//	printf("Enter message: ");
		//	scanf("%s\n", Message);
		//	while (1)
			if (send(Sock, Message, strlen(Message) + 1, 0) != SOCKET_ERROR) 
				printf("Sent!\n");
		 	else printf("Error of sending!\n");
		printf("\n");
	} while (1);

End:

	// �������� ������� � ��������� ������ � WinSock
	closesocket(Sock);
	WSACleanup();
	getch();
  return 7;
}