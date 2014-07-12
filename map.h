
void checkMouseMap();

void processMouseMap(SDL_Event event);

void processKeysMap(SDL_Event event);

int loadMapTexture(char * path);

void setDefaultTransform();

void setMove(float x,float y);

void setZoom(float x);

float gridToScreenX(float x, float y);

float gridToScreenY(float x, float y);

float screenToGridX(float x, float y);

float screenToGridY(float x, float y);

//actions

void actionMoveMap(void * arg);

