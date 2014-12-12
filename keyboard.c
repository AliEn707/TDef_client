#include "headers.h"

/*
╔══════════════════════════════════════════════════════════════╗
║ 										                       ║
║ created by Dennis Yarikov						                       ║
║ sep 2014									                       ║
╚══════════════════════════════════════════════════════════════╝
*/


static int shift=0;

void textShow(){
	sprintf(config.text.show,"%s", config.text.data);
	if (config.text.hide!=0)
		textHide(config.text.show);
}

void textAdd(char * dest, char* src, int pos){
	char tmp[ELEM_$TEXT];
	sprintf(tmp,"%s",dest+pos);
	dest[pos]=0;
	strcat(dest,src);
	strcat(dest,tmp);
}

void textRemChr(char * dest, int pos){
	char tmp[ELEM_$TEXT];
	sprintf(tmp,"%s",dest+pos+1);
	dest[pos]=0;
	strcat(dest,tmp);
}	

void textHide(char * text){
	int i;
	for(i=0;text[i]!=0;i++)
		text[i]='*';
}

//set '|' in editing text
void textSetCur(char* text, int cur){
	char tmp[ELEM_$TEXT];
	sprintf(tmp,"%s",text+cur);
	text[cur]='|';
	text[cur+1]=0;
	strcat(text,tmp);
}

//remove '|' from editing text
void textRemCur(char* text){
	char tmp[ELEM_$TEXT];
	int i;
	for (i=0;text[i]!='|';i++);
	sprintf(tmp,"%s",text+i+1);
	text[i]=0;
	strcat(text,tmp);
}
 
void textStop(){
	if (config.text.enable==0)
		return;
	config.text.enable=0; 
	config.text.hide=0;
	textRemCur(config.text.show);
	SDL_StopTextInput();
}

void textMove(int i){
	if (i<0){
		if (config.text.pos>0)
			config.text.pos--;
	}else{
		if (config.text.pos<strlen(config.text.data))
			config.text.pos++;
	}
	textRemCur(config.text.show);
	textSetCur(config.text.show,config.text.pos);
}


//actions

void actionKeyboard(void * arg){
	object * o=arg;
	switch (o->arg[0]){
		case 13://enter
			textStop();
			return;
		case 0: //shift
			shift=shift?0:1;
			return;
		case 8: //backspace
			config.text.pos--;
			textRemChr(config.text.data,config.text.pos);
			textShow();
			textSetCur(config.text.show,config.text.pos);
			return;
		case 62: //right
			textMove(1);
			return;
		case 60: //left
			textMove(-1);
			return;
	}
	
	if (config.text.pos<ELEM_$TEXT-1){
		char tmp[2];
		sprintf(tmp,"%c",o->arg[shift]);
		textAdd(config.text.data, tmp,config.text.pos);
		textShow();
		config.text.pos++;
		textSetCur(config.text.show,config.text.pos);
	}
}

void actionTextStart(void * arg){
//	object * o=arg;
	config.text.enable=1;
//	config.text.text[0]=0;
	config.text.pos=strlen(config.text.data);
	SDL_StartTextInput();
}

void actionTextLogin(void * arg){
	object * o=arg;
	config.text.data=config.public.login;
	config.text.show=o->elements[0].text;
	config.text.pos=strlen(config.text.data);
	textSetCur(config.text.show,config.text.pos);
	config.text.enable=1;
	SDL_StartTextInput();
}

void actionTextPass(void * arg){
	object * o=arg;
	config.text.hide=1;
	config.text.data=config.public.pass;
	config.text.show=o->elements[0].text;
	config.text.pos=strlen(config.text.data);
	textSetCur(config.text.show,config.text.pos);
	config.text.enable=1;
	SDL_StartTextInput();
}
