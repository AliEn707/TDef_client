#include "headers.h"
//http://www.unicode.org/charts/nameslist/

/*
╔══════════════════════════════════════════════════════════════╗
║ 										                       ║
║ created by Dennis Yarikov						                       ║
║ oct 2014									                       ║
╚══════════════════════════════════════════════════════════════╝
*/


#define FONTS 9

//textures for fonts 256x256 size

struct {
	char path[50];
	GLFONT font;
} fonts[FONTS]={
	{"latin",{0,0,0,0xa/*10 start*/,0x7f/*127 end*/,0}},//Latin 117 symbols
	{"latin_sup",{0,0,0,0xa1/*161 start*/,0xff/*255 end*/,0}},//Latin suplement 94
	{"cyrillic1",{0,0,0,0x400/*1024 start*/,0x45b/*1115 end*/,0}},//Cyrillic part1 91
	{"cyrillic2",{0,0,0,0x45c/*1116 start*/,0x4b7/*1207 end*/,0}},//Cyrillic part2 91
	{"cyrillic3",{0,0,0,0x4b8/*1208 start*/,0x4ff/*1279 end*/,0}},//Cyrillic part3 71 
	{"hiragana1",{0,0,0,0x3041/*12353 start*/,0x30e7/*12414 end*/,0}},//Hiragana part1 61 
	{"hiragana2",{0,0,0,0x30e8/*12415 start*/,0x309f/*12447 end*/,0}},//Hiragana part2 32 
	{"katagana1",{0,0,0,0x30a0/*12448 start*/,0x30dc/*12508 end*/,0}},//Katagana part1 60 
	{"katagana2",{0,0,0,0x30dd/*12509 start*/,0x30ff/*12543 end*/,0}},//Katagana part2 34 
	};

char2 str2_tmp[200];
char str_tmp[200];
int curr_tex_locale=-1;

char2 * strToStr2(const char * str){
	int i;
	for(i=0;str[i]!=0;i++){
		str2_tmp[i]=str[i];
	}
	str2_tmp[i]=0;
	return str2_tmp;
}

char * str2ToStr(char2 * str2){
	int i;
//	printf("str2 to str\n");
	for(i=0;str2[i]!=0;i++){
		str_tmp[i]=str2[i];
//		printf("\t %c\n",str_tmp[i]);
	}
	str_tmp[i]=0;
	return str_tmp;
}

int str2ln(char2 * str2){
	int i;
	for(i=0;str2[i]!=0;i++);
	return i;
}

GLFONT* localeFontGet(char2 chr){
	int i;
	for(i=0;i<FONTS;i++){
		if (chr>=fonts[i].font.IntStart && chr<=fonts[i].font.IntEnd)
			return &fonts[i].font;
	}
	return 0;
}

char* localeFontPath(char2 chr){
	int i;
	for(i=0;i<FONTS;i++){
		if (chr>=fonts[i].font.IntStart && chr<=fonts[i].font.IntEnd)
			return fonts[i].path;
	}
	return 0;
}
/*
//not used
void localeTexSet(char2 chr){
	int t;
	if ((t=localeTexGet(chr))==-1)
		return;
	if (t==curr_tex_locale)
		return;
	curr_tex_locale=t;
	//set locale texture
//	printf("set locale\n");
}
*/
int localeLoad(char * path){
	FILE *file; 
	if((file = fopen(path, "rb"))==0)
		perror("fopen localeLoad");
	char2 tmp[500];
	char *key;
	char2 chr;
	int i=0;
	short have_key=0;
	localeTextErase();
	fread(&chr, sizeof(chr), 1, file);	
	while (!feof(file)) {
		fread(&chr, sizeof(chr), 1, file);	
		if (have_key==0){
			if (chr==' '){
				tmp[i]=0;
				key=str2ToStr(tmp);
				have_key++;
				i=0;
			} else{
				tmp[i]=chr;
				i++;
			}
		}else{
			if (chr!='\r' && chr!='\n'){
				tmp[i]=chr;
				i++;
			}else {
				fread(&chr, sizeof(chr), 1, file);
				tmp[i]=0;
				char2 * value;
				i++;
				if((value=malloc(sizeof(char2)*i))==0)
					perror("malloc localeLoad");
				memcpy(value,tmp,sizeof(char2)*i);
				if(localeTextSet(key,value)!=0)
					perror("add locale text");
				
				printf("add %s \n",key);
				have_key=0;
				i=0;
			}
		}
//		printf("%x ", chr);
	}
	return 0;
}

/*
int main(){
	localeLoad("testutf16.txt");
	localeTextErase();
	localeTexSet(60);
	localeTexSet(60);
	localeTexSet(0x403);
}
*/