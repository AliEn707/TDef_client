int networkInit();

void networkExit();

TCPsocket networkConn(char * addr,int port);

TCPsocket networkConnMap(char * addr,int port);

int networkClose(TCPsocket sock);

int networkCheck(int sock);

int networkSetNonBlock(int sock);

int networkSetBlock(int sock);

int sendData(int socket,void * buf,int size);

int recvData(int socket,void * buf,int size);

int recvMesMap();