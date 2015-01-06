﻿//#include <windows.h> 
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

#define fakeMapClean() config.map.npc_array=0
#define NPC_TYPES 3

int parseArgs(char** argv){
	int i;
	for (i=0;argv[i]!=0;i++){
		if (strcmp("-m",argv[i])==0)
			sscanf(argv[++i],"%s",perf_test.map);
		if (strcmp("-s",argv[i])==0)
			sscanf(argv[++i],"%s",perf_test.server);
		if (strcmp("-p",argv[i])==0)
			sscanf(argv[++i],"%d",&perf_test.port);
		if (strcmp("-r",argv[i])==0)
			sscanf(argv[++i],"%d",&perf_test.$rooms);
		if (strcmp("-u",argv[i])==0)
			sscanf(argv[++i],"%d",&perf_test.$players);
		if (strcmp("-t",argv[i])==0)
			sscanf(argv[++i],"%d",&perf_test.time_per_serv);
		if (strcmp("-n",argv[i])==0)
			sscanf(argv[++i],"%d",&perf_test.npc_per_player);
		
	}
	return 0;
}
	
int askForRoom(){
	printf("ask for room\n");
	return 0;
}

#undef main
int main(int argc, char *argv[]){   
//int main(){   
	SDLNet_SocketSet socketset;
	signed i,j;
	
	printf("build at %s %s:%d\n",__DATE__,__FILE__,__LINE__);
	printf("Initializing.....");
	memset(&config,0,sizeof(config));
	memset(&perf_test,0,sizeof(perf_test));
	//defaults
	perf_test.port=11111;
	perf_test.$players=3;
	perf_test.$rooms=1;
	perf_test.time_per_serv=20000;//msec
	perf_test.npc_per_player=30;
	sprintf(perf_test.map,"4");
	if (parseArgs(argv)){
		printf("args error\n");
		return 2;
	}
	printf("using parameters:\nmax rooms %d\n",perf_test.$rooms);
	printf("server %s\n",perf_test.map);
	printf("map %s\n",perf_test.map);
	printf("players per room %d\n",perf_test.$players);
	printf("time per room %g sec\n",perf_test.time_per_serv/1000.0);
	printf("npc per player %d\n",perf_test.npc_per_player);
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
	short l_l=strlen(perf_test.map);
	SDLNet_TCP_Send(perf_test.sock,&l_l,sizeof(l_l));
	SDLNet_TCP_Send(perf_test.sock,perf_test.map,l_l);
*/	
	if ((perf_test.room=malloc(sizeof(room)*perf_test.$rooms))==0){
		perror("malloc rooms");
		return -4;
	}
	memset(perf_test.room,0,sizeof(room)*perf_test.$rooms);
	socketset = SDLNet_AllocSocketSet(perf_test.$rooms*perf_test.$players); 

	if (SDLNet_TCP_AddSocket(socketset, perf_test.sock)<0)
		perror("add to sockset");

	perf_test.main_running=1;
	printf("done\n");
	
//for test
	i=0;
	j=0;
	
	if ((perf_test.room[i].player[j].sock=networkConn("localhost",34140))==0){
		printf("error connect to server, exiting\n");
		return 1;
	}
	config.map.network.socket=perf_test.room[i].player[j].sock;
	networkMapAuth();
	perf_test.room[i].player[j].id=config.map.player_id;
	if (SDLNet_TCP_AddSocket(socketset, config.map.network.socket)<0)
		perror("add to sockset");
	
/////////////
	loadMap(perf_test.map);
	perf_test.room[i].player[j].npcs=config.map.npc_array;
	//fakeMapClean();
	for(i=0;i<perf_test.npc_per_player;i++){
		object o;
		o.arg[0]=rand()%NPC_TYPES;
		actionSpawnNpc(&o);
	}
	while(perf_test.main_running){ 
//		workSync(&time);
		config.global_count++;
//		SDL_Delay(100);
		int time=SDL_GetTicks();
		if (perf_test.time==0 || time-perf_test.time>perf_test.time_per_serv){
			askForRoom();
			perf_test.time=time;
		}
			
		if (SDLNet_CheckSockets(socketset, 10)!=0){

			if(SDLNet_SocketReady(perf_test.sock)) {
				int port;
				printf("next room ready\n");
				recvData(perf_test.sock,&port,sizeof(port));
				i=perf_test.$rooms;
				for(j=0;j<perf_test.$players;j++){
					if ((perf_test.room[i].player[j].sock=networkConn(perf_test.server,port))==0){
						printf("error connect to server, exiting\n");
						return 1;
					}
					config.map.network.socket=perf_test.room[i].player[j].sock;
					networkMapAuth();
					perf_test.room[i].player[j].id=config.map.player_id;
					perf_test.room[i].player[j].room_id=i;
					if (SDLNet_TCP_AddSocket(socketset, config.map.network.socket)<0)
						perror("add to sockset");
					loadMap(perf_test.map);
					perf_test.room[i].player[j].npcs=config.map.npc_array;
					
					for(i=0;i<perf_test.npc_per_player;i++){
						object o;
						o.arg[0]=rand()%NPC_TYPES;
						actionSpawnNpc(&o);
					}
					fakeMapClean();
				}
				perf_test.$rooms++;
				float latency=0;
				int _latency=0;
				printf("room latency:");
				for (i=0;i<perf_test.$rooms;i++){
					latency+=perf_test.room[i].latency;
					_latency++;
					perf_test.room[i].latency=0;
					perf_test.room[i]._latency=0;
					printf(" %g",perf_test.room[i].latency*1.0/perf_test.room[i]._latency);
				}
				printf("\n");
				printf("latency %g ms\n",latency/_latency);
			}

			//for all players
			for(i=0;i<perf_test.$rooms;i++){
				for(j=0;j<perf_test.$players;j++)
					if(SDLNet_SocketReady(config.map.network.socket)) {
						//add more players-bots
						config.map.network.socket=perf_test.room[i].player[j].sock;
						memset(&config.perf,0,sizeof(config.perf));
						config.perf.add_time=-1;
						config.map.npc_array=perf_test.room[i].player[j].npcs;
						config.map.player_id=perf_test.room[i].player[j].id;
						if(recvMesMap()<0){
							printf("connection lost\n");
							perf_test.main_running=0;
						}
						if (config.perf.npc!=0){
		//					if (config.perf.npc->owner!=0)
		//						printf("new npc of %d\n",config.perf.npc->owner);
							if (config.perf.npc->health<0){
								printf("npc dead\n");
								if (config.perf.npc->owner==config.map.player_id && config.perf.npc->type!=0){
									object o;
									o.arg[0]=rand()%NPC_TYPES;
									actionSpawnNpc(&o);
								}
							}
						}
						if (config.perf.add_time>=0){
							perf_test.room[i].latency+=config.perf.add_time;
							perf_test.room[i]._latency++;
						}
					}
				printf("latency %g ms\n",perf_test.room[i].latency*1.0/perf_test.room[i]._latency);
			}
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
