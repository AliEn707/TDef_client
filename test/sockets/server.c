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
  
  recv(Client, ClientName, 30, 0); // Получение имени компьютера клиента
	send(Client, PCName, strlen(PCName) + 1, 0); // Отправка имени этого компьютера (сервера)

	printf("Client ( \"%s\" ) has connected!\n", ClientName);

	// ***** Меню
	while (1){
			// Приём сообщения от клиента
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
	struct WSAData WSADat; // Свойства WinSock (результат функции WSAStartup)
  struct sockaddr_in sin; // Свойства(адрес) создаваемого сокета
	SOCKET Sock, Client; // Серверный и клиентский сокеты

	WSAStartup(0x0202,&WSADat); // Инициализация WinSock
								// 0x0202 - версия WinSock. Может быть 1.0, 1.1, 2.0, 2.2
								// WSADat - структура, куда будут занесены рез. инициализации
	gethostname(PCName, 30); // Получение имени текущего ПК

	sin.sin_family = AF_INET; // Тип адреса
	sin.sin_addr.s_addr = 0;  // IP-адрес сервера (пори создании сервера можно 0)
	sin.sin_port = htons(2803); // Номер порта сервера
	Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Создание сокета
	bind(Sock, (struct sockaddr_in*)&sin, sizeof(sin)); // Связывание созданного сокета с адресом sin
while (1){
	// ***** Ожиданеи клиента
	printf("Wait of client...\n");
	
	listen(Sock, SOMAXCONN); // Прослушивание сокета сервером (для подключения клиента)
	thr.Client = accept(Sock, (struct sockaddr_in*)&sin, 0); // Ожидание клиента
//=====================
  handle_of_comp = CreateThread(NULL,0,thread,&thr, 0,NULL);
  }
	// Закрытие сокетов и окончание работы с WinSock
	WaitForSingleObject(handle_of_comp ,-1);
  closesocket(Sock);
	closesocket(Client);
	WSACleanup();
	getch();
}