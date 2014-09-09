void loadMenu(menu * root,char* path);

int loadTexture(texture * t, char * path);
int loadMTexture(texture * t, char * path);

void realizeTexture(texture * t);

void loadMap(char* path);

int loadTex(texture * t, char * path, int(load)(char * path));

void loadFiles();

void cleanMap();

void cleanAll();
//
void setTestData();