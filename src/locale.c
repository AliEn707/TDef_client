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
#include "locales.data.h"	
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
	char fullpath[100];
	sprintf(fullpath,"../locale/%s.loc",path);
	if((file = fopen(fullpath, "rb"))==0){
		perror("fopen localeLoad");
		return 0;
	}
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
				
//				printf("add %s \n",key);
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