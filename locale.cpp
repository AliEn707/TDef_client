#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>

/*
╔══════════════════════════════════════════════════════════════╗
║ contaners for storing room data					                       ║
║ 										                       ║
║ created by Yaroslav Zotov						                       ║
║ edited by Dennis Yarikov						                       ║
║ oct 2014									                       ║
╚══════════════════════════════════════════════════════════════╝
*/


/*
extern "C" {
	#include "headers.h"
}
*/
//using namespace std;
typedef short char2;
std::map <std::string, char2*> locale;

extern "C" {
//#include "locale.h"
	
	int localeTextSet(const char * key, char2 * value) {
		std::string _key(key);
//		std::cout << _key << std::endl;
		if (locale.find(_key) == locale.end()) {
			locale[_key] = value;
			return 0;
		}
		return -1;
	}	

	char2 * strToStr2(const char * str);
	
	char2 * localeTextGet(const char * key) {
		//need to change
		return strToStr2(key);
		std::string _key(key);
		if (locale.find(_key) != locale.end())
			return locale[_key];
		return 0;
	}

	int localeTextErase() {
		if (locale.size() == 0)
			return 1;
		typedef std::map <std::string, char2*>::iterator it_type;
		for(it_type iterator = locale.begin(); iterator != locale.end(); iterator++) {
//			std::cout << iterator->second << std::endl;
			free(iterator->second);
		}
		locale.clear();
		return 0;
	}	
}
/*
extern "C" {
	void localeShow() {
		typedef std::map <std::string, char2*>::iterator it_type;
		for(it_type iterator = locale.begin(); iterator != locale.end(); iterator++) {
//			std::cout << iterator->second << std::endl;
			printf("key %s value %s\n",iterator->first.c_str(),str2ToStr(iterator->second));
		}
	}
	
}

int main() {
	FILE *file = fopen("utf16.txt", "rb");
	char2 aaa = 0;
	while (!feof(file)) {
		fread(&aaa, sizeof(aaa), 1, file);	
		printf("%x %x\n", (int)aaa,(int)'\r');
	}
	return 0;
}
*/