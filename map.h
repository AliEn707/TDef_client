
void checkMouseMap();

void processMouseMap(SDL_Event event);

void processKeysMap(SDL_Event event);

void processContKeysMap();

void setDefaultTransform();

void setMove(float x,float y);

void setZoom(float x);

float gridToScreenX(float x, float y);

float gridToScreenY(float x, float y);

float screenToGridX(float x, float y);

float screenToGridY(float x, float y);

void setActionMenu();

//actions

void actionShowWalkMap(void * arg);

void actionMoveMap(void * arg);

void actionZoomMap(void * arg);



