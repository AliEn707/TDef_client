
void processNodeAction();

void processObjectAction();

int checkMouseState();

void cursorMove(int xrel,int yrel);

void checkCursorBorder();

int getNewTexture();

int loadTexture(char * path);

void processEvent(SDL_Event event);

void graficsInit();

object * checkMouseOnObject(object * o);
