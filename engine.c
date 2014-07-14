#include "main.h"
#include "engine.h"
#include "menu.h"
#include "map.h"

void processNodeAction(){
	if(config.menu.selected==0)
		if (config.map.focus>=0)
			printf("action on %d node\n",config.map.focus);
}

void processObjectAction(int type,int key){
	if(config.menu.selected->touch!=0)
		if(config.menu.selected->action!=0)
			config.menu.selected->action(&config.menu.selected->arg);
	if(config.menu.selected->single!=0){
		if(type==KEYBOARD)
			config.global.keys[key]=0;
		if(type==MOUSE)
			config.global.mouse[key]=0;
	}
}

void processEvent(SDL_Event event){
	switch(event.type){
		case SDL_QUIT: 
			config.main_running = 0;
			break;
		case SDL_KEYDOWN:
			config.global.keys[event.key.keysym.sym]=1;
			switch(event.key.keysym.sym){ 
				case SDLK_ESCAPE: 
					config.main_running = 0; 
					break;
				case SDLK_m:
					actionToggleMenu(0);
					break;
			}
			if (config.menu.enable!=0)
				processKeysMenu(event);
			else
				if (config.map.enable!=0)
					processKeysMap(event);
			break;
		case SDL_KEYUP:
			config.global.keys[event.key.keysym.sym]=0;
			break;
		case SDL_MOUSEMOTION:
//			printf("%d %d\n",event.motion.xrel,event.motion.yrel);
			cursorMove(event.motion.xrel,-event.motion.yrel);
			checkCursorBorder();
			break;
		case SDL_MOUSEBUTTONDOWN:
			config.global.mouse[event.button.button]=1;
			if (config.menu.enable!=0)
				processMouseMenu(event);
			else
				if (config.map.enable!=0)
					processMouseMap(event);
			break;
		case SDL_MOUSEBUTTONUP:
			config.global.mouse[event.button.button]=0;
			break;
	}
}

void processKeyboard(){
	//global keys
	//mouse motion
	if (config.global.keys[SDLK_UP])
		cursorMove(0,5);
	if (config.global.keys[SDLK_DOWN])
		cursorMove(0,-5);
	if (config.global.keys[SDLK_LEFT])
		cursorMove(-5,0);
	if (config.global.keys[SDLK_RIGHT])
		cursorMove(5,0);
	//

	if (config.menu.enable==0)
		if (config.map.enable!=0)
			processContKeysMap();
				
};

int checkMouseOnElement(vec2 pos,element * e){
	if (cursor.state.x>=pos.x+e->position.x && 
			cursor.state.x<=pos.x+e->position.x+e->size.x &&
			cursor.state.y>=pos.y+e->position.y && 
			cursor.state.y<=pos.y+e->position.y+e->size.y){
		return 1;
	}
	return 0;
}


object * checkMouseOnObject(object * o){
	int i;
	for(i=0;i<o->elements_size;i++)
		if(checkMouseOnElement(o->position,&o->elements[i])!=0){
			if (o->focus!=0)
				o->in_focus=1;		
			return o;
		}
	if (o->focus!=0)
		o->in_focus=0;
	return 0;
}


void cursorMove(int xrel,int yrel){
	cursor.state.x+=xrel;
	cursor.state.y+=yrel;
}


void checkCursorBorder(){
	if (cursor.state.x<0)
		cursor.state.x=0;
	if (cursor.state.y<0)
		cursor.state.y=0;
	if (cursor.state.y>config.window_height)
		cursor.state.y=config.window_height;
	if (cursor.state.x>config.window_width)
		cursor.state.x=config.window_width;
	
}
//not used
void mouseMotion(){
	cursor.state.x+=(cursor.pos.x-cursor.prev.x)*cursor.sens;
	cursor.state.y+=-(cursor.pos.y-cursor.prev.y)*cursor.sens;
	checkCursorBorder();
	cursor.prev.x=cursor.pos.x;
	cursor.prev.y=cursor.pos.y;
}


int checkMouseState(){
//	mouseMotion();
	if (config.menu.enable!=0)
		checkMouseMenu(&config.menu.root);
	else
		if (config.map.enable!=0){
			checkMouseMap();
			if(checkMouseMenu(&config.map.screen_menu)==0)
				checkMouseMenu(&config.map.action_menu);
		}
//			if (config.menu.selected!=0)
//				config.menu.selected->in_focus=1;
//	printf("%d %d\n",cursor.state.x,cursor.state.y);
	
	
	
	return 0;
}


void cursorInit(){
	cursor.sens=1.0;
	cursor.color.r=1;
	cursor.color.g=1;
	cursor.color.b=1;
	cursor.state.x=0;
	cursor.state.y=config.window_height;
	
}



int loadGlobalTexture(char * path){
	config.textures[config.textures_size]=loadTGATexture(path);
	return config.textures[config.textures_size++];
}


int loadMapTexture(char * path){
	config.textures[config.map.textures_size]=loadTGATexture(path);
	return config.textures[config.map.textures_size++];
}


int getNewTexture(){
	glGenTextures(1,config.textures+config.textures_size);
	return config.textures[config.textures_size++];
}



void graficsInit(){

	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ){ 
	 	exit(1);
	} 
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 1);

	config.window = SDL_CreateWindow("TDef", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config.window_width, config.window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL ); //| SDL_WINDOW_FULLSCREEN
	
	SDL_GLContext glcontext = SDL_GL_CreateContext(config.window); // создаем контекст OpenGL
	
	if(config.window == NULL){
		exit(1);
	}
	
	cursorInit();
	
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,config.window_width,0,config.window_height,-10000,10000);
	glMatrixMode(GL_MODELVIEW); 
	
//	glEnable(GL_POINT_SMOOTH);
//	glEnable(GL_LINE_SMOOTH);
//	glEnable(GL_POLYGON_SMOOTH);
	
	
}


