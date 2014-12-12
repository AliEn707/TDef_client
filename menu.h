
void setScreenMessage(char * mes);

int checkMouseMenu(menu * root);

void processMouseMenu(SDL_Event event);

void processKeysMenu(SDL_Event event);



//actions
void actionTestMenu(void * arg); //test

void actionToggleMenu(void * arg); //toggle

void actionExit(void * arg);

void actionExitRoom(void * arg); 

void actionAuth(void * arg);
//for debug
void actionMapTest(void * arg);
