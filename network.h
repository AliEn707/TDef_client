void networkInit();

void networkExit();

int networkConn(char * addr,int port);

int networkClose(int sock);

int networkCheck(int sock);

int networkSetNonBlock(int sock);

int networkSetBlock(int sock);

int sendData(int socket,void * buf,int size);

int recvData(int socket,void * buf,int size);
