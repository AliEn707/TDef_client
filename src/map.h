
void mapStart(char * path);

void checkMenuMap();

void drawMenuMap();

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

float gridToMinimapX(float x,float y);

float gridToMinimapY(float x,float y);

void setActionMenu();

//actions

void actionShowWalkMap(void * arg);

void actionMoveMap(void * arg);
void actionZoomMap(void * arg);

void actionDropTower(void * arg);

void actionSpawnNpc(void * arg);

void actionMinimapToggle(void * arg);

void actionMinimap(void * arg);

void actionTowerSpawnBrush(void * arg);

void actionNpcSpawnBrush(void * arg);

void actionTowerRemove(void * arg);

//brush

void brushTowerCreate();

 void processBrush();
 
 void dropBrush();