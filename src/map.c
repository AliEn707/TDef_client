#include "headers.h"


/*
╔══════════════════════════════════════════════════════════════╗
║ 										                       ║
║ created by Dennis Yarikov						                       ║
║ jun 2014									                       ║
╚══════════════════════════════════════════════════════════════╝
*/


/*
#define amenu config.map.action_menu
void setActionMenu(){
	int i,j;
	amenu.$objects=TOWER_SET_SIZE;
	if ((amenu.objects=malloc(amenu.$objects*sizeof(object)))==0)
		perror("malloc setActionMenu");
	for(i=0;i<amenu.$objects;i++){
		amenu.objects[i].$elements=1;
		if ((amenu.objects[i].elements=malloc(amenu.objects[i].$elements*sizeof(element)))==0)
			perror("malloc setActionMenu");
		for(j=0;j<amenu.objects[i].$elements;j++){
		//	amenu.objects[i].elements[j].
		}
	}
}
#undef amenu
*/
void mapStart(char * path){
	config.loading.enable=1;
	printf("start map %s\n",path);
	cleanMap();
	loadMap(path);
	loadMapGrafics(path);
	loadMenu(&config.map.screen_menu,"../data/mapmenu.cfg");
//	setActionMenu();
	loadMenu(&config.map.tower_menu,"../data/towermenu.cfg");
	loadMenu(&config.map.npc_menu,"../data/npcmenu.cfg");
	if (networkConnMap(config.map.network.server,config.map.network.port)!=0){
		if (networkMapAuth()==0){
			config.map.enable=1;
			config.public.enable=0;
			config.map.worker=workerMapStart();
			config.map.connector=connectorMapStart();
		}
	}else{
		config.map.enable=1;
		config.public.enable=0;
		setTestData();
	}
	config.public.player.status=PLAYER_NONE;
	config.loading.enable=0;
}

static inline void menuCheckDraw(int (action)(menu * root)){
	action(&config.map.tower_menu);
	action(&config.map.npc_menu);
	action(&config.map.screen_menu);
	if (config.map.action_menu.enable!=0)
		action(&config.map.action_menu);
}

void checkMenuMap(){
	menuCheckDraw(checkMouseMenu);
}

void drawMenuMap(){
	menuCheckDraw(drawMenu);
}


void checkMouseMap(){
	if (config.menu.enable!=0)
		return;
	if (config.map.enable==0 || config.auth==0){
		config.map.focus=-1;
		return;
	}
	//check icons
	vec2 pos={ICON_POS_X,ICON_POS_Y};
	if (cursor.state.x>=pos.x && cursor.state.x<=pos.x+ICON_SIZE && cursor.state.y>=pos.y && cursor.state.y<=pos.y+ICON_SIZE){
		config.menu.selected=&config.map.base_obj;
		return;
	}	
	if (cursor.state.x>=H_ICON_POS_X && cursor.state.x<=H_ICON_POS_X+H_ICON_SIZE && cursor.state.y>=H_ICON_POS_Y && cursor.state.y<=H_ICON_POS_Y+H_ICON_SIZE){
		config.menu.selected=&config.map.hero_obj;
		return;
	}	
	//check minimap
	if(config.map.minimap.enable!=0) 
		if (cursor.state.x>config.options.window.width-MINIMAP_AREA_WIDTH-MINIMAP_AREA_SHIFT*2-SCREEN_OFFSET && 
				cursor.state.y>config.options.window.height-MINIMAP_AREA_HEIGHT-MINIMAP_AREA_SHIFT*2-SCREEN_OFFSET &&
				cursor.state.x<config.options.window.width-SCREEN_OFFSET &&
				cursor.state.y<config.options.window.height-SCREEN_OFFSET){
			if (cursor.state.x>config.options.window.width-SCREEN_OFFSET-MINIMAP_AREA_WIDTH && 
					cursor.state.y>config.options.window.height-SCREEN_OFFSET-MINIMAP_AREA_HEIGHT){
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
	config.map.focus=to2di(x,y);
	if (config.map.grid[to2di(x,y)].buildable>0 && config.map.brush.type==BRUSH_TOWER_SPAWN){
		cursor.text=config.map.text;
		sprintf(cursor.text,"build tower");
		return;
	}
	//config.map.focus=-1;
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
	float xl=gridToScreenX(0,0);//-SCREEN_OFFSET;
	float xr=gridToScreenX(config.map.grid_size,config.map.grid_size);//+SCREEN_OFFSET;
	float yd=gridToScreenY(config.map.grid_size,0);//-SCREEN_OFFSET;
	float yu=gridToScreenY(0,config.map.grid_size);//+SCREEN_OFFSET;
	//printf("xl %g %g\n",yd, yu);
	if (xl>0){
		config.global.transform.translate.x-=xl;
		s=2;
	}
	if (xr<config.options.window.width){
		config.global.transform.translate.x-=xr-(config.options.window.width);
		s=3;
	}
	if (yu<config.options.window.height){
		config.global.transform.translate.y-=yu-(config.options.window.height);
		s=4;
	}
	if (yd>0){
		config.global.transform.translate.y-=yd;
		s=5;
	}
	float x=xr-xl;
	float y=yu-yd;
	float scale=1;
	if (x<(config.options.window.width) || y<(config.options.window.height)){
		if (x-(config.options.window.width)<y-(config.options.window.height))
			scale=(config.options.window.width)/x;
		else
			scale=(config.options.window.height)/y;
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
		setMove(0,CAMERA_ZOOM*s*screenToGridX(config.options.window.width/2.0f,config.options.window.height/2.0));;
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
	//get x y for viewport with borders
	x=(x-SCREEN_OFFSET)*config.options.window.width/(config.options.window.width-SCREEN_OFFSET*2);
	y=(y-SCREEN_OFFSET)*config.options.window.height/(config.options.window.height-SCREEN_OFFSET*2);
		return -(500*y)/(707*sy)+(500*x)/(707*sx)+
		(500*sx*ty-500*sy*tx)/
		(707*sx*sy);
}

float screenToGridY(float x, float y){
	//get x y for viewport with borders
	x=(x-SCREEN_OFFSET)*config.options.window.width/(config.options.window.width-SCREEN_OFFSET*2);
	y=(y-SCREEN_OFFSET)*config.options.window.height/(config.options.window.height-SCREEN_OFFSET*2);
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

	
/*
void setActionMenu(){
	printf("action on %d node (tower %d)\n",	config.map.focus,
								config.map.grid[config.map.focus].tower_id);
	tower* t; 
	t=getTowerById(config.map.grid[config.map.focus].tower_id);
	if (t->id==0){
		int i;
		for(i=0;i<config.map.action_menu.$objects;i++){
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
		sprintf(config.map.action_menu.objects[config.map.action_menu.$objects-1].text,
				"destroy tower");
	}
	config.map.action_menu.enable=1;
}*/
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

void actionDropTower(void * arg){
	object * o=arg;
	int * p=(o->arg);
	if (config.map.network.socket==0)
		return;
	char mtype=MSG_DROP_TOWER;
	if(SDLNet_TCP_Send(config.map.network.socket,&mtype,sizeof(mtype))<0)
		perror("send spawnTower");
//	else printf("send %d %d %d",mtype,p[0],p[1]);
	SDLNet_TCP_Send(config.map.network.socket,&p[0],sizeof(int));
//	send(config.map.network.socket,sizeof(mtype),0);
}

void actionSpawnNpc(void * arg){
	object * o=arg;
	int * p=(o->arg);
	if (config.map.network.socket==0)
		return;
	char mtype=MSG_SPAWN_NPC;
	if(SDLNet_TCP_Send(config.map.network.socket,&mtype,sizeof(mtype))<0)
		perror("send spawnNpc");
//	else printf("send %d %d %d",mtype,p[0],p[1]);

	SDLNet_TCP_Send(config.map.network.socket,&p[0],sizeof(int));
//	SDLNet_TCP_Send(config.map.network.socket,&p[1],sizeof(int));
//	send(config.map.network.socket,sizeof(mtype),0);
}

void actionMinimapToggle(void * arg){
	if (config.map.minimap.used!=0)
		config.map.minimap.enable=(config.map.minimap.enable+1)&1;
	//sdsd
}

void actionMinimap(void * arg){
	float dsize=MINIMAP_AREA_WIDTH;
	//vec2 lcScreen={screenToGridX(SCREEN_OFFSET,(config.options.window.height-SCREEN_OFFSET)/2),screenToGridY(SCREEN_OFFSET,(config.options.window.height-SCREEN_OFFSET)/2)};
	vec2 centerScreen={screenToGridX((config.options.window.width-SCREEN_OFFSET)/2,(config.options.window.height-SCREEN_OFFSET)/2),screenToGridY((config.options.window.width-SCREEN_OFFSET)/2,(config.options.window.height-SCREEN_OFFSET)/2)};
	//vec2 zeroMap={config.global.transform.translate.x,config.global.transform.translate.y};
	vec2 zeroMinimap={(config.options.window.width-SCREEN_OFFSET)-dsize,(config.options.window.height-SCREEN_OFFSET)-dsize/2};
	vec2 minimap_shift={cursor.state.x-(zeroMinimap.x+gridToMinimapX(centerScreen.x,centerScreen.y)),
					cursor.state.y-(zeroMinimap.y+gridToMinimapY(centerScreen.x,centerScreen.y))};
	float shift_scale=2;
	//fabs(((zeroMap.x-0.01)/(gridToMinimapX(lcScreen.x,lcScreen.y)+0.001)));//some hack to get non zero
	config.global.transform.translate.x-=minimap_shift.x*shift_scale;
	config.global.transform.translate.y-=minimap_shift.y*shift_scale;
	globalTransformCorrection();
}

void actionTowerSpawnBrush(void * arg){
	object * o=arg;
	config.map.brush.id=(short)o->arg[0];
	config.map.brush.type=BRUSH_TOWER_SPAWN;
	config.map.brush.action=brushTowerCreate;
	printf("set Brush Tower button id %d\n",config.map.brush.id);
}

void actionBaseIcon(void * arg){
	printf("base icon action\n");
}

void actionHeroIcon(void * arg){
	printf("hero icon action\n");
	object * o=arg;
	config.map.brush.id=(short)o->arg[0];
	config.map.brush.type=BRUSH_HERO_POSITION;
	config.map.brush.action=brushHeroPos;
}

void actionSetTarget(void * arg){
	object * o=arg;
	int * p=(o->arg);
	if (config.map.network.socket==0)
		return;
	char mtype=MSG_SPAWN_NPC;
	if(SDLNet_TCP_Send(config.map.network.socket,&mtype,sizeof(mtype))<0)
		perror("send spawnNpc");
	mtype=p[0];
	if(SDLNet_TCP_Send(config.map.network.socket,&mtype,sizeof(mtype))<0)
		perror("send spawnNpc");

}

///brush

#define CONTEXT_MENU_BUTTONS 3
//textures init array
static texture * context_textures[CONTEXT_MENU_BUTTONS]={&config.map.tex[REMOVE_OBJECT],&config.map.tex[BUILDABLE],&config.map.tex[WALKABLE]};
static void(*context_actions[CONTEXT_MENU_BUTTONS])(void*)={actionDropTower,actionTestMenu,actionTestMenu};
static char * context_obj_texts[CONTEXT_MENU_BUTTONS]={"remove tower"};
//proceed click on node
void processBrush(){
	printf("click on node %d\n",config.map.focus);
	//defaul actions
	if (config.map.grid[config.map.focus].tower_id!=0){
		tower* t=getTowerById(config.map.grid[config.map.focus].tower_id);
		if (t!=0)
			if (t->owner==config.map.player_id && t->type!=BASE){ //may act only on player own towers
				menu *m_m=contextMenuInit(CONTEXT_MENU_BUTTONS,
										1,54, 
										context_textures,
										context_actions,	//to set array must be (void(*[])(void*))
										(int[CONTEXT_MENU_BUTTONS][4]){{config.map.focus},
																	{config.map.focus}},
										context_obj_texts,
										0,
										0
										); 
				//TODO: add texts to init
				m_m->enable=1;
	//			setScreenMessage("#something");
				dropBrush();
				config.map.selected_tower=t->id;
			}
	}
	
	if (config.map.brush.action!=0)
		config.map.brush.action();
//	config.map.focus;
//	config.map.brush;
}

void dropBrush(){
	config.map.brush.id=0;
	config.map.brush.type=0;
	config.map.brush.action=0;
	printf("clear Brush\n");
}

void brushTowerCreate(){
//	printf("send %hd %hd\n",config.map.focus,config.map.brush.id);
	if (config.map.grid[config.map.focus].buildable<=0){
//		dropBrush();
		setScreenMessage("#cant_create_tower_here");
		return;
	}
	//TODO: add money check
	if (config.map.network.socket==0)
		return;
	char mtype=MSG_SPAWN_TOWER;
	if(SDLNet_TCP_Send(config.map.network.socket,&mtype,sizeof(mtype))<0)
		perror("send spawnTower");
//	else printf("send %d %d %d",mtype,p[0],p[1]);
	SDLNet_TCP_Send(config.map.network.socket,&config.map.focus,sizeof(int));
	SDLNet_TCP_Send(config.map.network.socket,&config.map.brush.id,sizeof(config.map.brush.id)); //short
//	send(config.map.network.socket,sizeof(mtype),0);
}

void brushHeroPos(){
	float x=screenToGridX(cursor.state.x,cursor.state.y);
	float y=screenToGridY(cursor.state.x,cursor.state.y);
	int pos=to2di(x,y);
	if (pos<0 || pos>=sqr(config.map.grid_size))
		return;
	printf("send hero to %d node\n",pos);
	//TODO: pos check
	if (config.map.network.socket==0)
		return;
	char mtype=MSG_MOVE_HERO;
	if(SDLNet_TCP_Send(config.map.network.socket,&mtype,sizeof(mtype))<0)
		perror("send spawnTower");
//	else printf("send %d %d %d",mtype,p[0],p[1]);
	SDLNet_TCP_Send(config.map.network.socket,&pos,sizeof(pos));
	dropBrush();
}
