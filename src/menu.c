﻿#include "headers.h"


/*
╔══════════════════════════════════════════════════════════════╗
║ 										                       ║
║ created by Dennis Yarikov						                       ║
║ jul 2014									                       ║
╚══════════════════════════════════════════════════════════════╝
*/


void setScreenMessage(char * mes){
	sprintf(config.message,"%s",mes);
}

int checkMouseMenu(menu* root){
	if (root==0)
		return 0;
//	if (root->enable==0)
//		return 0;
	menu* m=0;
	m=root;
	int i;
	int o=0;
	for(i=0;i<config.menu.depth;i++)
		m=&m->submenu[config.menu.path[i]];
	for(i=0;i<m->$objects;i++){
		object* tmp;
		if ((tmp=checkMouseOnObject(&m->objects[i]))!=0){
//			config.menu.selected=tmp;
			//if (*tmp->text!=0)
//				cursor.text=tmp->text;
			o++;
		}
	}
	return o;
}


void processMouseMenu(SDL_Event event){	
	if (event.button.button==SDL_BUTTON_LEFT){
		processObjectAction(MOUSE,SDL_BUTTON_LEFT);
		//config.global.mouse[event.button.button]=0;
	}
}

void processKeysMenu(SDL_Event event){
	if (event.key.keysym.sym==SDLK_SPACE){
		processObjectAction(KEYBOARD,SDLK_SPACE);
		//config.global.keys[event.key.keysym.sym]=0;
	}
}
//context menu

menu* contextMenuInit(int obj,int elems,float size,texture**t,void(**a_a)(void*_arg),int arg[][4],char**otext,vec2*tpos,char**etext){
	menu* m=&config.global.context_menu;
	int i;
	float alpha=2*M_PI/obj;
	float r=obj>2?(1.41421356f*size)/(2*sinf(alpha)):size;
	realizeMenu(m);
	m->$objects=obj;
	m->objects=malloc(m->$objects*sizeof(object));
	memset(m->objects,0,m->$objects*sizeof(object));
	for(i=0;i<m->$objects;i++){
		m->objects[i].$elements=elems;
		m->objects[i].elements=malloc(m->objects[i].$elements*sizeof(element));
		memset(m->objects[i].elements,0,m->objects[i].$elements*sizeof(element));
		int j;
		//for positions
		//n=7;n.times {|i| c=(360.0/n); p c+(180-c)/2+(c*i)}
		m->objects[i].position.x=cursor.state.x-size/2+r*cosf(alpha*(i+0.5f)+M_PI/2);
		m->objects[i].position.y=cursor.state.y-size/2-r*sinf(alpha*(i+0.5f)+M_PI/2);
		if (a_a!=0){
			if (a_a[i]!=0){
				m->objects[i].action=a_a[i];
				m->objects[i].touch=1;
				m->objects[i].single=1;
			}
			if (arg!=0)
				memcpy(m->objects[i].arg,arg[i],sizeof(int[4]));
		}
		if (otext!=0)
			if (otext[i]!=0)
				sprintf(m->objects[i].text,"%s",otext[i]);
		for(j=0;j<m->objects[i].$elements;j++){
			m->objects[i].elements[j].color.r=1.0f;
			m->objects[i].elements[j].color.g=1.0f;
			m->objects[i].elements[j].color.b=1.0f;
			m->objects[i].elements[j].color.a=1.0f;
			m->objects[i].elements[j].size.x=size;
			m->objects[i].elements[j].size.y=size;
			//set texture
			if (t!=0)
				if (t[i]!=0)
					memcpy(&m->objects[i].elements[j].tex,t[i],sizeof(texture));
			if (etext!=0)
				if (etext[i]!=0)
					sprintf(m->objects[i].elements[j].text,"%s",etext[i]);
			if (tpos!=0)
				memcpy(&m->objects[i].elements[j].text_position,&tpos[i],sizeof(vec2));
		}
	}
	return m;
}
//TODO: add erase 

void contextMenuDisable(){
	config.global.context_menu.enable=0;
}


void targetMenuInit(menu * m, int players){
	int i;
	int j=0;
	int elems=1;
	int dsize=SCREEN_OFFSET;
	realizeMenu(m);
	m->$objects=players+3;
	m->objects=malloc(m->$objects*sizeof(object));
	memset(m->objects,0,m->$objects*sizeof(object));
	for(i=0;i<m->$objects;i++){
		m->objects[i].$elements=elems;
		m->objects[i].elements=malloc(m->objects[i].$elements*sizeof(element));
		memset(m->objects[i].elements,0,m->objects[i].$elements*sizeof(element));
		
		m->objects[i].position.x=config.options.window.width-dsize*(players-(i-(m->$objects-players)))-SCREEN_OFFSET;
		m->objects[i].position.y=config.options.window.height-dsize;
		
		m->objects[i].elements[j].color.r=1.0f;
		m->objects[i].elements[j].color.g=1.0f;
		m->objects[i].elements[j].color.b=1.0f;
		m->objects[i].elements[j].color.a=1.0f;
		m->objects[i].elements[j].size.x=dsize;
		m->objects[i].elements[j].size.y=dsize;
		m->objects[i].elements[j].text_position.y=0;
		m->objects[i].elements[j].text_position.x=4;
		
		sprintf(m->objects[i].elements[j].text,"#deselected_target");
		sprintf(m->objects[i].text,"%d",i-1);
		if (i-1==-1)
			sprintf(m->objects[i].text,"#Hero_target");
		if (i-1==0)
			sprintf(m->objects[i].text,"#Rand_target");
		if (i-1>0)
			m->objects[i].disabled=1;
		m->objects[i].action=actionSetTarget;
		m->objects[i].touch=1;
		m->objects[i].focus=1;
		m->objects[i].single=1;
		
		m->objects[i].arg[0]=i-1;
		m->objects[i].elements[j].tex.frames=-1;
	}
	m->enable=1;
}

void targetMenuEnable(menu * m,int i){
	m->objects[i+2].disabled=0;
}

void targetMenuDisable(menu * m,int i){
	m->objects[i+2].disabled=1;	
}

void targetMenuSelected(menu * m,int i){
	int j;
	for(j=0;j<m->$objects;j++){
		if (i+1==j){
			//set i to selected
			sprintf(m->objects[j].elements[0].text,"#selected_target");
		}else{
			//deselect other
			sprintf(m->objects[j].elements[0].text,"#deselected_target");
		}
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

void actionExitRoom(void * arg){
	config.loading.enable=1;
	config.map.enable=0;
	config.menu.enable=0;
	config.loading.enable=0;
}

void actionMapStart(void * arg){
//	config.loading.enable=1;
//	mapStart("global");
//	mapStart("testbig");
	
//	sprintf(config.map.network.server,"%s",??);
//	config.map.network.port= ??
	mapStart("test");
//	config.map.enable=1;
//change to params for new connection	
//	config.loading.enable=0;
}

void actionMapTest(void * arg){
	config.auth++;
	config.map.enable=0;
	config.public.enable=0;
//	config.map.enable=1;
	{
		FILE * file=fopen("connect.txt","r");
		fscanf(file,"%s\n",config.map.network.server);
		fscanf(file,"%d\n",&config.map.network.port);
		fclose(file);
	}
	mapStart("4");//test");
}

void actionAuth(void * arg){
	config.auth++;
//	actionMapStart(0);
	config.map.enable=0;
	config.public.enable=0;
	publicStart();
}

void actionTextTest(void * arg){
	config.text.enable=1;
	SDL_StartTextInput();
}

