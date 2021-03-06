
int sign(float x);
//
splash* getSplashNew();

npc* getNpcById(int id);

tower* getTowerById(int id);

bullet* getBulletById(int id);
//
void frameSync(unsigned int *time);

void workSync(unsigned int *time);

void tickSync(unsigned int *time);

float dirToAngle(vec2 v);

void processNodeAction();

void processObjectAction(int type,int key);

void processEvent(SDL_Event event);

void processKeyboard();

int processMouse();

void cursorMove(int xrel,int yrel);

void checkCursorBorder();

int getNewTexture();

int loadGlobalTexture(FILE * path);

int loadMapTexture(FILE * path);

void processEvent(SDL_Event event);

void graficsInit();

object * checkMouseOnObject(object * o);
