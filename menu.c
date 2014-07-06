#include "main.h"
#include "menu.h"
#include "engine.h"


int checkMouseMenu(){
	menu* m=0;
	m=&config.menu.root;
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
	switch(event.button.button){
				case SDL_BUTTON_LEFT:
					if(config.menu.selected!=0)
						if(config.menu.selected->touch!=0)
							config.menu.selected->action(&config.menu.selected->arg);
					break;
			}
}

void processKeysMenu(SDL_Event event){
	
}


//actions

void actionTestMenu(void * arg){
	printf("test\n");
}

void actionToggleMenu(void * arg){
	config.menu.enable=config.menu.enable==0?1:0;
}
