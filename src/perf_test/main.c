//#include <windows.h> 
#include "main.h"


/*
#include "../main.h"
#include "../locale.h"
#include "../tga.h"
#include "../drawer.h"
#include "../engine.h"
#include "../file.h"
#include "../menu.h"
#include "../map.h"
#include "../threads.h"
#include "../network.h"
#include "../lights.h"
#include "../public.h"
*/


int parseArgs(){

	return 0;
}
	


#undef main
int main(int argc, char *argv[]){   
//int main(){   
	SDLNet_SocketSet socketset;
	
	printf("build at %s %s:%d\n",__DATE__,__FILE__,__LINE__);
	printf("Initializing.....");
	memset(&config,0,sizeof(config));
	memset(&perf_test,0,sizeof(perf_test));
	//defaults
	perf_test.port=11111;
	perf_test.$servers=50;
	perf_test.time_per_serv=20000;//msec
	
	if (parseArgs(argv)){
		printf("args error\n");
		return 2;
	}
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0 ){ 
		printf("SDL_Init\n");
	 	exit(1);
	} 
	
	if(SDLNet_Init()==-1){
		printf("SDLNet_Init: %s\n",SDLNet_GetError());
		return -1;
	}
	srand(time(0));
	//requared
	config.time_per_tick=1;
	loadMenu(&config.map.screen_menu,"../data/mapmenu.cfg");
	loadMenu(&config.map.tower_menu,"../data/towermenu.cfg");
	loadMenu(&config.map.npc_menu,"../data/npcmenu.cfg");
//////////
/*	
//not used yet
	if ((perf_test.sock=networkConn(perf_test.server,perf_test.port))==0){
		printf("error connect to server, exiting\n");
		return 1;
	}
*/	
	socketset = SDLNet_AllocSocketSet(perf_test.$servers); 
	
	perf_test.main_running=1;
	printf("done\n");
	
//for test
	TCPsocket sock;
	if ((sock=networkConn("localhost",34140))==0){
		printf("error connect to server, exiting\n");
		return 1;
	}
	config.map.network.socket=sock;
	networkMapAuth();
	if (SDLNet_TCP_AddSocket(socketset, config.map.network.socket)<0)
		perror("add to sockset");
	
/////////////	
	loadMap("4");
/*	//not very good
	config.map.grid_size=40;
	if ((config.map.grid=malloc(sizeof(gnode)*(config.map.grid_size*config.map.grid_size+1)))==0){
		printf("malloc grid\n");
		return -3;
	}
	memset(config.map.grid,0,sizeof(gnode)*(config.map.grid_size*config.map.grid_size+1));
*/
	while(perf_test.main_running){ 
//		workSync(&time);
		config.global_count++;
//		SDL_Delay(100);
		if(SDLNet_CheckSockets(socketset, 3)!=0){
			//config.map.network.socket=
			memset(&config.perf,0,sizeof(config.perf));
			config.perf.add_time=-1;
			if(recvMesMap()<0){
				printf("connection lost\n");
				perf_test.main_running=0;
			}
			if (config.perf.npc!=0){
				if (config.perf.npc->owner!=0)
					printf("new npc of %d\n",config.perf.npc->owner);
				if (config.perf.npc->health<0)
					printf("npc dead\n");
			}
			if (config.perf.add_time>=0)
				printf("%d\n",config.perf.add_time);
		}
	}
	
	cleanMap();
	realizeMenu(&config.map.screen_menu);
	realizeMenu(&config.map.tower_menu);
	realizeMenu(&config.map.npc_menu);
	SDL_Delay(20);
	SDLNet_FreeSocketSet(socketset);
	
	printf("exit\n");
	SDLNet_Quit();
	SDL_Quit(); // завершаем работу SDL и выходим
	return 0;
}
