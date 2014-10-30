

///events
void checkMenuPublic();

void processKeysPublic(SDL_Event event);


///drawing
void publicDraw();


///network
void publicStart();


///threads
SDL_Thread* workerPublicStart();

SDL_Thread* connectorPublicStart();


//actions

void actionPublicMove(void * arg);