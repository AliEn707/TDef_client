#include "main.h"
#include "engine.h"
#include "menu.h"
#include "map.h"
#include "network.h"
typedef
struct worker_arg{
	int q;
}worker_arg;

int workerMap(void *ptr){
	worker_arg * arg=ptr;
	int socket=config.map.network.socket;
	Uint32 time=0;
	printf("done\n");
	while(config.map.enable){
		tickSync(&time);
		//get data from server
		while (recvMesMap()!=-1);
		//process data on client
		int i;
		for(i=0;i<config.map.npc_max;i++)
			if (config.map.npc_array[i].id!=0){
				config.map.npc_array[i].position.x+=config.map.npc_array[i].direction.x;
				config.map.npc_array[i].position.y+=config.map.npc_array[i].direction.y;
				if (config.map.npc_array[i].health<=0 &&
						config.map.npc_array[i].current_tex!=TEX_DESTROY){
					config.map.npc_array[i].anim_ended=0;
					config.map.npc_array[i].id=0; 
//					config.map.npc_array[i].current_tex=TEX_DESTROY; 
				}
				if (config.map.npc_array[i].current_tex==TEX_DESTROY && 
						config.map.npc_array[i].anim_ended!=0)
					config.map.npc_array[i].id=0; 
			}
		for(i=0;i<config.map.tower_max;i++)
			if (config.map.tower_array[i].id!=0){
				if (config.map.tower_array[i].health<=0){
					config.map.tower_array[i].id=0;
					//change to set animation
				}
			}
		for(i=0;i<config.map.bullet_max;i++)
			if(config.map.bullet_array[i].id!=0){
				config.map.bullet_array[i].position.x+=config.map.bullet_array[i].direction.x;
				config.map.bullet_array[i].position.y+=config.map.bullet_array[i].direction.y;
				if (config.map.bullet_array[i].detonate!=0)
					config.map.bullet_array[i].id=0;
			}
		
	}
	printf("exit workerMap\n");
}

SDL_Thread* workerMapStart(){
	worker_arg arg;
	printf("start worker....");
	return SDL_CreateThread(workerMap, "WorkerMap", (void*)&arg);
}