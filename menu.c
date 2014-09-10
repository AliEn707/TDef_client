#include "headers.h"

int checkMouseMenu(menu* root){
	if (root==0)
		return 0;
	menu* m=0;
	m=root;
	int i;
	int o=0;
	config.menu.selected=0;
	for(i=0;i<config.menu.depth;i++)
		m=&m->submenu[config.menu.path[i]];
	for(i=0;i<m->objects_size;i++){
		object* tmp;
		if ((tmp=checkMouseOnObject(&m->objects[i]))!=0){
			config.menu.selected=tmp;
			if (*tmp->text!=0)
				cursor.text=tmp->text;
			o++;
		}
	}
	return o;
}


void processMouseMenu(SDL_Event event){	
	if (event.button.button==SDL_BUTTON_LEFT){
		if(config.menu.selected!=0)
			processObjectAction(MOUSE,SDL_BUTTON_LEFT);
		//config.global.mouse[event.button.button]=0;
	}
}

void processKeysMenu(SDL_Event event){
	if (event.key.keysym.sym==SDLK_SPACE){
		if(config.menu.selected!=0)
			processObjectAction(KEYBOARD,SDLK_SPACE);
		//config.global.keys[event.key.keysym.sym]=0;
	}
}


//actions

void actionTestMenu(void * arg){
	printf("test\n");
}

void actionToggleMenu(void * arg){
	config.menu.enable=config.menu.enable==0?1:0;
}

void actionExit(void * arg){
	config.main_running=0;
}

void actionMapStart(void * arg){
//	config.loading.enable=1;
//	mapStart("global");
//	mapStart("testbig");
	mapStart("test");
//change to params for new connection	
//	config.loading.enable=0;
}

void actionAuth(void * arg){
	config.auth++;
	actionMapStart(0);
}

void actionTextTest(void * arg){
	config.text.enable=1;
	SDL_StartTextInput();
}

void actionTextStart(void * arg){
	config.text.enable=1;
	config.text.text[0]=0;
	config.text.pos=strlen(config.text.text);
	SDL_StartTextInput();
}

