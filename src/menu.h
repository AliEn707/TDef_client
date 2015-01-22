
void setScreenMessage(char * mes);

int checkMouseMenu(menu * root);

void processMouseMenu(SDL_Event event);

void processKeysMenu(SDL_Event event);

menu* contextMenuInit(int obj,int elems,float size,texture ** t,void(**a_a)(void*_arg),int arg[][4],char**otext,vec2*tpos,char**etext);
void contextMenuDisable();

void targetMenuInit(menu * m, int players);
//actions
void actionTestMenu(void * arg); //test

void actionToggleMenu(void * arg); //toggle

void actionExit(void * arg);

void actionExitRoom(void * arg); 

void actionAuth(void * arg);
//for debug
void actionMapTest(void * arg);
