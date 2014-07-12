#include "main.h"
#include "menu.h"
#include "engine.h"


int checkMouseMenu(menu* root){
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
			o++;
		}
	}
	return o;
}


void processMouseMenu(SDL_Event event){	
	if (event.button.button==SDL_BUTTON_LEFT){
		if(config.menu.selected!=0)
			processObjectAction();
		config.global.mouse[event.button.button]=0;
	}
}

void processKeysMenu(SDL_Event event){
	if (event.key.keysym.sym==SDLK_SPACE){
		if(config.menu.selected!=0)
			processObjectAction();
		config.global.keys[event.key.keysym.sym]=0;
	}
}


//actions

void actionTestMenu(void * arg){
	printf("test\n");
}

void actionToggleMenu(void * arg){
	config.menu.enable=config.menu.enable==0?1:0;
}
