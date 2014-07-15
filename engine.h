
void processNodeAction();

void processObjectAction(int type,int key);

int checkMouseState();

void cursorMove(int xrel,int yrel);

void checkCursorBorder();

int getNewTexture();

int loadGlobalTexture(char * path);

int loadMapTexture(char * path);

void processEvent(SDL_Event event);

void graficsInit();

object * checkMouseOnObject(object * o);
