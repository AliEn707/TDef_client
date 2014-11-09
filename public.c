#include "headers.h"

/*
╔══════════════════════════════════════════════════════════════╗
║ 										                       ║
║ created by Dennis Yarikov						                       ║
║ oct 2014									                       ║
╚══════════════════════════════════════════════════════════════╝
*/

#define sendData SDLNet_TCP_Send
#define recvPublic(b,c) if (recvData(config.public.network.socket,b,c)<=0) return -1

#define checkMask(x,y) x&y

#define SIZE_OF_PASSWD 16

typedef
struct worker_arg{
	int q;
}worker_arg;


//universal function to work with menus
static inline void $CheckDraw(int (action)(menu * root)){
	if (config.public.player.status==PLAYER_IN_LOBBY){
		action(&config.public.lobby);
	}
	if (config.public.player.status==PLAYER_AT_MAP){
		action(&config.public.map_main);
		action(&config.public.map_info);
		action==checkMouseMenu?eventsCheck():eventsDraw();
	}
	action(&config.public.main);
}

///input 
void checkMenuPublic(){
	$CheckDraw(checkMouseMenu);
/*	
	checkMouseMenu(&config.map.screen_menu);
	checkMouseMenu(&config.map.tower_menu);
	checkMouseMenu(&config.map.npc_menu);
	if (config.map.action_menu.enable!=0)
		checkMouseMenu(&config.map.action_menu);
*/
}

void processKeysPublic(SDL_Event event){
/*  //add keys to addition for map keys
	if(event.key.keysym.sym==SDLK_SPACE) 
		processNodeAction();
	if(event.key.keysym.sym==SDLK_z) 
		actionShowWalkMap(0);
	if(event.key.keysym.sym==SDLK_m) 
		actionMinimapToggle(0);
*/	
	//add another
}

///drawing

void publicDraw(){
	$CheckDraw(drawMenu);
}


///connection


//need to correct
int publicAuth(){
	//fake auth
	const char name[10]= "teoto";
	const char passwd[20]= "testwdhnehodktrfff";
	//size of name   $a for size of a
	int $name;
	$name=strlen(name);
	//not connected
	if (sendData(config.public.network.socket,&$name,sizeof($name))<0){
		config.public.network.socket=0;
		return 1;
	}
	if (sendData(config.public.network.socket,name,$name)<0){
		config.public.network.socket=0;
		return 1;
	}
	
	//get salt to add to passwd and generate new
	recvData(config.public.network.socket,&$name,sizeof($name));
	printf("get %d for salt\n",$name);
	
	sendData(config.public.network.socket,passwd,SIZE_OF_PASSWD);
	recvData(config.public.network.socket,&$name,sizeof($name));
	//we get normal answer
	if ($name!=0)
		return 0;
	//set error message	
	setScreenMessage("#auth_error");
	SDLNet_TCP_Close(config.public.network.socket);
	config.public.network.socket=0;	
	return 1;
}

int recvMesPublic(){
	//must do recv
	char mes; 
	int bitmask;
	event * e_e;
	short l_l;
		
	recvPublic(&mes,sizeof(mes));
	printf("get mes %d\n",mes);
	if (mes==MESSAGE_GAME_START){
		memset(config.map.network.server,0,sizeof(config.map.network.server));
//		printf("mes game start\n");
		//get sizeof hostname
		recvPublic(&l_l,sizeof(l_l));
		//get hostname
		recvPublic(config.map.network.server,l_l);
		//get port
		recvPublic(&config.map.network.port,sizeof(config.map.network.port));
		printf("ready to start game\n");
		recvPublic(&bitmask,sizeof(bitmask));
		e_e=eventsGet(bitmask);
		printf("start map %s on port %d\n",e_e->map,config.map.network.port);
		if (config.map.enable==0)
			mapStart(e_e->map);
		//add some stuff
//		if (checkMask(bitmask,BM_PLAYER_STATUS)){
//			recvPublic(&config.public.player.status,sizeof(config.public.player.status));
//		}
		return 1;
	}
	if (mes==MESSAGE_PLAYER_CHANGE){
		printf("mes %d\n",mes);
		recvPublic(&bitmask,sizeof(bitmask));
		printf("bitmask %d\n",bitmask);
		//add some stuff
//		if (checkMask(bitmask,BM_PLAYER_STATUS)){
//			recvPublic(&config.public.player.status,sizeof(config.public.player.status));
//		}
		return 1;
	}
	if (mes==MESSAGE_EVENT_CHANGE){
		printf("mes event\n");
		recvPublic(&bitmask,sizeof(bitmask));
//		printf("get %d\n",bitmask);
		e_e=eventsAdd(bitmask);
		//add 0 check
		recvPublic(&e_e->$rooms,sizeof(e_e->$rooms));
		recvPublic(&l_l,sizeof(l_l));
		recvPublic(e_e->map,l_l);
//		printf("get %d\n",bitmask);
		//get other data
		e_e->o->arg[0]=e_e->id;
		//test
		e_e->position.x=0.5;
		e_e->position.y=0.5;
		
		e_e->o->position.x=config.public._map_info.position+e_e->position.x*config.public._map_info.size;
		e_e->o->position.y=config.public._map_info.position+e_e->position.y*config.public._map_info.size;
		
		
		return 1;
	}
	printf("something strange %d \n",mes);
	return 1;
}

void publicStart(){
//	char mes;
	config.loading.enable=1;
	cleanMap();
	loadMap("public");
	loadMapGrafics("public");
	//lobby menu
	if (config.public.lobby.$objects==0)
		loadMenu(&config.public.lobby,"../data/lobbymenu.cfg");
	if (config.public.main.$objects==0)
		loadMenu(&config.public.main,"../data/publicmain.cfg");
	if (config.public.map_main.$objects==0)
		loadMenu(&config.public.map_main,"../data/publicmap.cfg");
	if (config.public.map_info.$objects==0)
		eventsFillMapMenu();
//	loadMenu(&config.map.screen_menu,"../data/mapmenu.cfg");
//	setActionMenu();
//	loadMenu(&config.map.tower_menu,"../data/towermenu.cfg");
//	loadMenu(&config.map.npc_menu,"../data/npcmenu.cfg");
	//check allready connected
/*	if (config.public.network.socket!=0){
		mes=MESSAGE_MOVE;
		if (sendData(config.public.network.socket,&mes,sizeof(mes))<=0)
			config.public.network.socket=0;
		mes=MESSAGE_LOBBY;
		if (config.public.network.socket!=0)
			if (sendData(config.public.network.socket,&mes,sizeof(mes))<=0)
				config.public.network.socket=0;
	}
*/	
	if (config.public.network.socket==0){
		config.public.network.socket=networkConn(PUBLIC_SERVER,PUBLIC_PORT);
	}
	
	if (config.public.network.socket!=0){
		publicAuth();
	}
	
	if (config.public.network.socket==0){
		printf("error connecting to public\n");
		setScreenMessage("#public_conn_error");
	}
	
	//if socket == 0 we could not connect
	if (config.public.network.socket!=0){
		config.public.enable=1;
//			config.map.worker=workerPublicStart();
			config.map.connector=connectorPublicStart();
	}else{
		config.auth=0;
		//set error message
	}
	config.loading.enable=0;
}

///threads

int connectorPublic(void *ptr){
//	worker_arg * arg=ptr;
	SDLNet_SocketSet socketset;
	socketset = SDLNet_AllocSocketSet(1); 
	if (SDLNet_TCP_AddSocket(socketset, config.public.network.socket)<0){
		perror("add to sockset");
		exit(1);
	}
	printf("done\n");
	while(config.auth){
	//	config.map.time_now=SDL_GetTicks();
		//get data from server
		if(SDLNet_CheckSockets(socketset, 10)!=0)
			if(recvMesPublic()<0){
				perror("network error");
				setScreenMessage("network error");
				config.auth=0;
				config.public.enable=0;
			}
	}
	SDLNet_FreeSocketSet(socketset);
	SDLNet_TCP_Close(config.public.network.socket);
	config.public.network.socket=0;
	printf("exit connectorPublic\n");
	return 0;
}


SDL_Thread* connectorPublicStart(){
	worker_arg arg;
	printf("start connector....\n");
	return SDL_CreateThread(connectorPublic, "ConnectorPublic", (void*)&arg);
}

int workerPublic(void *ptr){
//	worker_arg * arg=ptr;
	Uint32 time=0;
	SDL_Delay(900);
	printf("done\n");
	while(config.public.enable){
		tickSync(&time);

	}
	printf("exit workerMap\n");
	return 0;
}

SDL_Thread* workerPublicStart(){
	worker_arg arg;
	printf("start wrker....\n");
	return SDL_CreateThread(workerPublic, "WorkerPublic", (void*)&arg);
}


//actions

void actionEventChoose(void * arg){
	object * o=arg;
	int * p=(o->arg);
	config.public._map_info.event=*p;
	sprintf(config.public._map_info.$rooms,"%d",config.public._map_info.event);
}

void actionPublicMove(void * arg){
	object * o=arg;
	int * p=(o->arg);
/*	
	if(SDLNet_TCP_Send(config.map.network.socket,&mtype,sizeof(mtype))<0)
		perror("send spawnNpc");
*/
	config.public.player.status=*p;
}

void actionRoomMgmt(void * arg){
	object * o=arg;
	int * p=(o->arg);
	char mtype;
	if (config.public.network.socket==0)
		return;
	printf("room act %d\n",config.public._map_info.event);
	mtype=MESSAGE_ROOM_ACT;
	if (SDLNet_TCP_Send(config.public.network.socket,&mtype,sizeof(mtype))<0)
		perror("send spawnNpc");
	
	mtype=(*p==1?MESSAGE_CREATE_ROOM:MESSAGE_FAST_ROOM);
	
	if (SDLNet_TCP_Send(config.public.network.socket,&mtype,sizeof(mtype))<0)
		perror("send spawnNpc");
	if (SDLNet_TCP_Send(config.public.network.socket,&config.public._map_info.event,sizeof(config.public._map_info.event))<0)
		perror("send spawnNpc");
	
}