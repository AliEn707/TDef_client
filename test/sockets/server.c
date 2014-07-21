#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "mswsock.lib")

typedef struct data {
  SOCKET Client;
  } data;


DWORD WINAPI thread( LPVOID lpParam ){
  char PCName [30], ClientName[30], Message[200];
	SOCKET Client=((data*)lpParam)[0].Client;
  
  recv(Client, ClientName, 30, 0); // ��������� ����� ���������� �������
	send(Client, PCName, strlen(PCName) + 1, 0); // �������� ����� ����� ���������� (�������)

	printf("Client ( \"%s\" ) has connected!\n", ClientName);

	// ***** ����
	while (1){
			// ���� ��������� �� �������
			if (recv(Client, Message, 200, 0) != SOCKET_ERROR) { 
				printf("%d %s\n",Client, Message);
				//getch();
			}
			else {printf("Error of getting!\n");return 1;}
	
		}
//==============
  return 0;
  }

  
  
void main(void) {
  data thr;
  HANDLE handle_of_comp;
	char PCName [30], ClientName[30], Message[200];
	struct WSAData WSADat; // �������� WinSock (��������� ������� WSAStartup)
  struct sockaddr_in sin; // ��������(�����) ������������ ������
	SOCKET Sock, Client; // ��������� � ���������� ������

	WSAStartup(0x0202,&WSADat); // ������������� WinSock
								// 0x0202 - ������ WinSock. ����� ���� 1.0, 1.1, 2.0, 2.2
								// WSADat - ���������, ���� ����� �������� ���. �������������
	gethostname(PCName, 30); // ��������� ����� �������� ��

	sin.sin_family = AF_INET; // ��� ������
	sin.sin_addr.s_addr = 0;  // IP-����� ������� (���� �������� ������� ����� 0)
	sin.sin_port = htons(2803); // ����� ����� �������
	Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // �������� ������
	bind(Sock, (struct sockaddr_in*)&sin, sizeof(sin)); // ���������� ���������� ������ � ������� sin
while (1){
	// ***** �������� �������
	printf("Wait of client...\n");
	
	listen(Sock, SOMAXCONN); // ������������� ������ �������� (��� ����������� �������)
	thr.Client = accept(Sock, (struct sockaddr_in*)&sin, 0); // �������� �������
//=====================
  handle_of_comp = CreateThread(NULL,0,thread,&thr, 0,NULL);
  }
	// �������� ������� � ��������� ������ � WinSock
	WaitForSingleObject(handle_of_comp ,-1);
  closesocket(Sock);
	closesocket(Client);
	WSACleanup();
	getch();
}