#include "headers.h"

/*
╔══════════════════════════════════════════════════════════════╗
║ 										                       ║
║ created by Dennis Yarikov						                       ║
║ oct 2014									                       ║
╚══════════════════════════════════════════════════════════════╝
*/

typedef
struct worker_arg{
	int q;
}worker_arg;

int publicRestoreConn(){
	return 0;
}

int publicAuth(){
	//if already connected
	
	//if not connected
	
	//set error message
	setScreenMessage("auth error");
	return 0;
}

int recvMesPublic(){
	return 1;
}

void publicStart(){
	config.loading.enable=1;
	cleanMap();
	loadMap("public");
	loadMapGrafics("public");
//	loadMenu(&config.map.screen_menu,"../data/mapmenu.cfg");
//	setActionMenu();
//	loadMenu(&config.map.tower_menu,"../data/towermenu.cfg");
//	loadMenu(&config.map.npc_menu,"../data/npcmenu.cfg");
	//check allready connected
	if (config.public.network.socket==0){
		config.public.network.socket=networkConn(PUBLIC_SERVER,PUBLIC_PORT);
		//check auth
		if (publicAuth()!=0){
			SDLNet_TCP_Close(config.public.network.socket);
			config.public.network.socket=0;
		}
	}
	//if socket == 0 we could not connect
	if (config.public.network.socket!=0){
//			config.map.worker=workerPublicStart();
			config.map.connector=connectorPublicStart();
	}else{
		//set error message
	}
	config.loading.enable=0;
}

int connectorPublic(void *ptr){
//	worker_arg * arg=ptr;

	SDLNet_SocketSet socketset;
	socketset = SDLNet_AllocSocketSet(1); 
	if (SDLNet_TCP_AddSocket(socketset, config.public.network.socket)<0)
		perror("add to sockset");
	
	printf("done\n");
	while(config.map.enable){
	//	config.map.time_now=SDL_GetTicks();
		//get data from server
		if(SDLNet_CheckSockets(socketset, 5)==0)
			SDL_Delay(5);
		if(recvMesPublic()<0){
			perror("network error");
			setScreenMessage("network error");
			config.map.enable=0;
		}
	}
	SDLNet_FreeSocketSet(socketset);
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
	while(config.map.enable){
		tickSync(&time);
		int i;

	}
	printf("exit workerMap\n");
	return 0;
}

SDL_Thread* workerPublicStart(){
	worker_arg arg;
	printf("start worker....\n");
	return SDL_CreateThread(workerPublic, "WorkerPublic", (void*)&arg);
}
