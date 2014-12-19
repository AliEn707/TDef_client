

//functions need to use different locales
//typedef short char2;


//.c
//buf str2 set globaly in .c file

char2 * strToStr2(const char * str);

char * str2ToStr(char2 * str2);

GLFONT* localeFontGet(char2 chr);

char * localeFontPath(char2 chr);

int localeLoad(char * path);


//.cpp
//uses 1 locale hash for program, it set globaly in .cpp file 

//add string value to hash by key
int localeTextSet(const char * key, char2 * value);

//get string from hash by key
char2 * localeTextGet(const char * key);

int localeTextErase();

