#include "headers.h"


//

void mapStart(char * path){
	config.loading.enable=1;
	cleanMap();
	loadMap(path);
	loadMenu(&config.map.screen_menu,"../data/mapmenu.cfg");
	loadMenu(&config.map.action_menu,"../data/actionmenu.cfg");
	if (networkConnMap(config.map.network.server,config.map.network.port)!=0){
		config.map.worker=workerMapStart();
		config.map.connector=connectorMapStart();
	}else{
		setTestData();
	}
	config.loading.enable=0;
}

void checkMouseMap(){
	if (config.menu.enable!=0)
		return;
	if (config.map.enable==0){
		config.map.focus=-1;
		return;
	}
	float x=screenToGridX(cursor.state.x,cursor.state.y);
	float y=screenToGridY(cursor.state.x,cursor.state.y);
	if (x<0 || y<0 || x>config.map.grid_size || y>config.map.grid_size){
		config.map.focus=-1;
		return;
	}
	if (config.map.grid[to2di(x,y)].buildable>0){
		config.map.focus=to2di(x,y);
		cursor.text=config.map.text;
		sprintf(cursor.text,"build tower");
		return;
	}
	config.map.focus=-1;
}

void processMouseMap(SDL_Event event){	
	//priority on screen buttons
	if(event.button.button==SDL_BUTTON_LEFT){
		processNodeAction();
//		processObjectAction(MOUSE,SDL_BUTTON_LEFT);
	}
}


void processKeysMap(SDL_Event event){
	if(event.key.keysym.sym==SDLK_SPACE) 
		processNodeAction();
	if(event.key.keysym.sym==SDLK_z) 
		actionShowWalkMap(0);
	
}


void processContKeysMap(){
	if (config.global.keys[SDLK_w]!=0)
		setMove(-CAMERA_SPEED,0);
	if (config.global.keys[SDLK_s]!=0)
		setMove(CAMERA_SPEED,0);
	if (config.global.keys[SDLK_d]!=0)
		setMove(0,-CAMERA_SPEED);
	if (config.global.keys[SDLK_a]!=0)
		setMove(0,CAMERA_SPEED);
	if (config.global.keys[SDLK_q]!=0)
		setZoom(0.8);
	if (config.global.keys[SDLK_e]!=0)
		setZoom(-0.8);
	
	//add another
		if(config.global.mouse[SDL_BUTTON_LEFT]==1 ||
				config.global.keys[SDLK_SPACE]==1)
			if(config.menu.selected!=0)
				processObjectAction(config.global.mouse[SDL_BUTTON_LEFT]==1?MOUSE:KEYBOARD,
								config.global.mouse[SDL_BUTTON_LEFT]==1?SDL_BUTTON_LEFT:SDLK_SPACE);
}


int globalTransformCorrection(){
	int s=0;
	float xl=gridToScreenX(0,0);
	float xr=gridToScreenX(config.map.grid_size,config.map.grid_size);
	float yd=gridToScreenY(config.map.grid_size,0);
	float yu=gridToScreenY(0,config.map.grid_size);
	if (xl>0){
		config.global.transform.translate.x-=xl;
		s=2;
	}
	if (xr<config.window_width){
		config.global.transform.translate.x-=xr-config.window_width;
		s=3;
	}
	if (yu<config.window_height){
		config.global.transform.translate.y-=yu-config.window_height;
		s=4;
	}
	if (yd>0){
		config.global.transform.translate.y-=yd;
		s=5;
	}
	float x=xr-xl;
	float y=yu-yd;
	float scale=1;
	if (x<config.window_width || y<config.window_height){
		if (x-config.window_width<y-config.window_height)
			scale=config.window_width/x;
		else
			scale=config.window_height/y;
		s=1;
	}
	config.global.transform.scale*=scale;
	return s;
	/*
	s is correction of 
	1 zoom
	2 left
	3 right
	4 up
	5 down
	*/
}

void setDefaultTransform(){
	config.global.transform.scale=100;
	globalTransformCorrection();
}



void setMove(float x,float y){
	config.global.transform.translate.x+=y; //do not change!
	config.global.transform.translate.y+=x;
	globalTransformCorrection();
}

void setZoom(float x){
	config.global.transform.scale+=x;
	if (config.global.transform.scale<=0)
		config.global.transform.scale=0.2;
	float s=-1;
	if (x<0)
		s*=-1;
	if(globalTransformCorrection()!=1)
		setMove(0,s*screenToGridX(config.window_width/2.0f,config.window_height/2.0));;
}


#define tx config.global.transform.translate.x
#define ty config.global.transform.translate.y
#define sx config.global.transform.scale
#define sy (sx/2)

//take to map.c file

//from grid to screen
//  y
//0
//  x

float gridToScreenX(float x, float y){
	return sx*(0.707*(x)+0.707*(y))+tx;
}

float gridToScreenY(float x, float y){
	return sy*(0.707*(y)-0.707*(x))+ty;
}

//from screen to grid
//sreen 
//y
//0 x

float screenToGridX(float x, float y){
		return -(500*y)/(707*sy)+(500*x)/(707*sx)+
			(500*sx*ty-500*sy*tx)/
			(707*sx*sy);
}

float screenToGridY(float x, float y){
		return (500*y)/(707*sy)+(500*x)/(707*sx)-
			(500*sx*ty+500*sy*tx)/
			(707*sx*sy);
}
#undef tx
#undef ty
#undef sx
#undef sy

void setActionMenu(){
	printf("action on %d node (tower %d)\n",
								config.map.focus,
								config.map.grid[config.map.focus].tower_id);
	tower* t; 
	t=getTowerById(config.map.grid[config.map.focus].tower_id);
	if (t->id==0){
		int i;
		sprintf(config.map.action_menu.objects[0].text,
				"create tower");
		for(i=0;i<config.map.action_menu.objects_size;i++){
			config.map.action_menu.objects[i].arg[0]=config.map.focus;
		}
		config.map.action_menu.objects[0].arg[1]=1;
		config.map.action_menu.objects[0].action=actionSpawnTower;
	}else{
		sprintf(config.map.action_menu.objects[0].text,
				"upgrade");
		sprintf(config.map.action_menu.objects[config.map.action_menu.objects_size-1].text,
				"destroy tower");
	}
	config.map.action_menu.enable=1;
}
//

void actionShowWalkMap(void * arg){
	config.map.show_walk=config.map.show_walk==1?0:1;
}

void actionMoveMap(void * arg){
	int * p=(int*)arg;
//	printf("%g %g\n",(float)p[0],(float)p[1]);
	setMove((float)p[0],(float)p[1]);
}

void actionZoomMap(void * arg){
	int * p=(int*)arg;
	setZoom(p[0]*1.0/p[1]);
}

void actionSpawnTower(void * arg){
	int * p=(int*)arg;
	if (config.map.network.socket==0)
		return;
	char mtype=MSG_SPAWN_TOWER;
	if(SDLNet_TCP_Send(config.map.network.socket,&mtype,sizeof(mtype))<0)
		perror("send spawnTower");
//	else printf("send %d %d %d",mtype,p[0],p[1]);
	SDLNet_TCP_Send(config.map.network.socket,&p[0],sizeof(int));
	SDLNet_TCP_Send(config.map.network.socket,&p[1],sizeof(int));
//	send(config.map.network.socket,sizeof(mtype),0);
}
