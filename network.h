void networkInit();

void networkExit();

int networkConn(char * addr,int port);

int networkClose(int sock);

int networkCheck(int sock);

int networkSetNonBlock(int sock);

int networkSetBlock(int sock);

