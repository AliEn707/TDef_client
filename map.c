#include "headers.h"


//

void mapStart(char * path){
	config.loading.enable=1;
	cleanMap();
	loadMap(path);
	loadMapGrafics(path);
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
	if (config.map.enable==0 || config.auth==0){
		config.map.focus=-1;
		return;
	}
	if(config.map.minimap.enable!=0) 
		if (cursor.state.x>config.window_width-MINIMAP_AREA_WIDTH-MINIMAP_AREA_SHIFT*2 && 
				cursor.state.y>config.window_height-MINIMAP_AREA_HEIGHT-MINIMAP_AREA_SHIFT*2){
			if (cursor.state.x>config.window_width-MINIMAP_AREA_WIDTH && 
					cursor.state.y>config.window_height-MINIMAP_AREA_HEIGHT){
				config.menu.selected=&config.map.minimap.obj;
			}
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
	if(event.key.keysym.sym==SDLK_m) 
		actionMinimapToggle(0);
	
	//add another
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
		setZoom(CAMERA_ZOOM);
	if (config.global.keys[SDLK_e]!=0)
		setZoom(-CAMERA_ZOOM);
	
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
//	printf("%g %g\n",config.global.transform.translate.x,config.global.transform.translate.y);
}

void setZoom(float x){
	config.global.transform.scale+=x;
	if (config.global.transform.scale<=0)
		config.global.transform.scale=0.2;
	float s=-1;
	if (x<0)
		s*=-1;
	if(globalTransformCorrection()!=1)
		setMove(0,CAMERA_ZOOM*s*screenToGridX(config.window_width/2.0f,config.window_height/2.0));;
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


float gridToMinimapX(float x,float y){
	return (0.707*y+0.707*x)*MINIMAP_SIZE/config.map.grid_size;
}

float gridToMinimapY(float x,float y){
	return (0.5*(0.707*y-0.707*x)*MINIMAP_SIZE/config.map.grid_size+MINIMAP_AREA_WIDTH/4);
}

	

void setActionMenu(){
	printf("action on %d node (tower %d)\n",	config.map.focus,
								config.map.grid[config.map.focus].tower_id);
	tower* t; 
	t=getTowerById(config.map.grid[config.map.focus].tower_id);
	if (t->id==0){
		int i;
		for(i=0;i<config.map.action_menu.objects_size;i++){
			config.map.action_menu.objects[i].arg[0]=config.map.focus;
			config.map.action_menu.objects[i].arg[1]=i+1;
			config.map.action_menu.objects[i].action=actionSpawnTower;
			sprintf(config.map.action_menu.objects[i].text,
				"create %d",i+1);
		
		}
		//config.map.action_menu.objects[0].arg[1]=1;
		//config.map.action_menu.objects[0].action=actionSpawnTower;
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
	object * o=arg;
//	printf("%g %g\n",(float)p[0],(float)p[1]);
//	setMove((float)p[0],(float)p[1]);
	setMove((float)o->arg[0],(float)o->arg[1]);
}

void actionZoomMap(void * arg){
	object * o=arg;
//	setZoom(p[0]*1.0/p[1]);
	setZoom(o->arg[0]*CAMERA_ZOOM*0.85);
}

void actionSpawnTower(void * arg){
	object * o=arg;
	int * p=(o->arg);
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

void actionMinimapToggle(void * arg){
	if (config.map.minimap.used!=0)
		config.map.minimap.enable=(config.map.minimap.enable+1)&1;
	//sdsd
}

void actionMinimap(void * arg){
	float dsize=MINIMAP_AREA_WIDTH;
	vec2 lcScreen={screenToGridX(0,config.window_height/2),screenToGridY(0,config.window_height/2)};
	vec2 centerScreen={screenToGridX(config.window_width/2,config.window_height/2),screenToGridY(config.window_width/2,config.window_height/2)};
	vec2 zeroMap={config.global.transform.translate.x,config.global.transform.translate.y};
	vec2 zeroMinimap={config.window_width-dsize,config.window_height-dsize/2};
	vec2 minimap_shift={cursor.state.x-(zeroMinimap.x+gridToMinimapX(centerScreen.x,centerScreen.y)),
					cursor.state.y-(zeroMinimap.y+gridToMinimapY(centerScreen.x,centerScreen.y))};
	float shift_scale=-((zeroMap.x-0.01)/(gridToMinimapX(lcScreen.x,lcScreen.y)+0.01));//some hack to get non zero
	config.global.transform.translate.x-=minimap_shift.x*shift_scale;
	config.global.transform.translate.y-=minimap_shift.y*shift_scale;
	globalTransformCorrection();
}


