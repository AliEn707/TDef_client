﻿#include "headers.h"

/*
╔══════════════════════════════════════════════════════════════╗
║ 										                       ║
║ created by Dennis Yarikov						                       ║
║ aug 2014									                       ║
╚══════════════════════════════════════════════════════════════╝
*/


typedef
struct worker_arg{
	int q;
}worker_arg;



float dirToAngle(vec2 v){
	float length=sqrt(sqr(v.x)+sqr(v.y));
	if (length==0)
		return 0;
	float cos=v.x/length;
	float angle=acosf(cos);
	if (v.y<0)
		angle=M_PI*2-angle;
	return angle;
}


int getWalkTex(vec2 v){
	float ang=dirToAngle(v);
	float p8=M_PI/8.0f;
	//need to correct
	if (ang>15*p8 || ang<=p8)
		return TEX_WALK_RIGHT_DOWN;
	if (ang>p8 && ang<=3*p8)
		return TEX_WALK_RIGHT;
	if (ang>3*p8 && ang<=5*p8)
		return TEX_WALK_RIGHT_UP;
	if (ang>5*p8 && ang<=7*p8)
		return TEX_WALK_UP;
	if (ang>7*p8 && ang<=9*p8)
		return TEX_WALK_LEFT_UP;
	if (ang>9*p8 && ang<=11*p8)
		return TEX_WALK_LEFT;
	if (ang>11*p8 && ang<=13*p8)
		return TEX_WALK_LEFT_DOWN;
	if (ang>13*p8 && ang<=15*p8)
		return TEX_WALK_DOWN;
	return 0;
}

int getAttackTex(vec2 v){
	float ang=dirToAngle(v);
	float p8=M_PI/8;
	//need to correct
	if (ang>15*p8 || ang<=p8)
		return TEX_ATTACK_RIGHT_DOWN;
	if (ang>p8 && ang<=3*p8)
		return TEX_ATTACK_RIGHT;
	if (ang>3*p8 && ang<=5*p8)
		return TEX_ATTACK_RIGHT_UP;
	if (ang>5*p8 && ang<=7*p8)
		return TEX_ATTACK_UP;
	if (ang>7*p8 && ang<=9*p8)
		return TEX_ATTACK_LEFT_UP;
	if (ang>9*p8 && ang<=11*p8)
		return TEX_ATTACK_LEFT;
	if (ang>11*p8 && ang<=13*p8)
		return TEX_ATTACK_LEFT_DOWN;
	if (ang>13*p8 && ang<=15*p8)
		return TEX_ATTACK_DOWN;
	return 0;
}


int workerMap(void *ptr){
//	worker_arg * arg=ptr;
	Uint32 time=0;
//	SDL_Delay(900);
	printf("done\n");
	while(config.map.enable){
		tickSync(&time);
		int i;
		for(i=0;i<config.map.npc_max;i++)
			if (config.map.npc_array[i].id!=0){
				///////////////////////////
//				if (config.map.npc_array[i].current_tex==TEX_ATTACK){
				if (config.map.npc_array[i].attack_prepare!=0){
//					printf("attack");
					config.map.npc_array[i].attack_prepare=0;
					config.map.npc_array[i].current_tex=getAttackTex(config.map.npc_array[i].direction);
					config.map.npc_array[i].tex[config.map.npc_array[i].current_tex].current_frame=0;
					config.map.npc_array[i].tex[config.map.npc_array[i].current_tex].lf_delay_counter=0;
					config.map.npc_array[i].anim_ended=0;
					memset(&config.map.npc_array[i].direction,0,sizeof(vec2));
					continue;
				}
				if (checkNpcTexAttack(config.map.npc_array[i].current_tex) &&
						config.map.npc_array[i].anim_ended!=0){
					config.map.npc_array[i].current_tex=getWalkTex(config.map.npc_array[i].direction);
					continue;
				}
				if (config.map.npc_array[i].health<=0 &&
						config.map.npc_array[i].current_tex!=TEX_DESTROY){
					config.map.npc_array[i].anim_ended=0;
					
					config.map.npc_array[i].$$$=0; 
					memset(&config.map.npc_array[i],0,sizeof(npc));
//					config.map.npc_array[i].current_tex=TEX_DESTROY; 
					continue;
				}
				if (config.map.npc_array[i].current_tex==TEX_DESTROY && 
						config.map.npc_array[i].anim_ended!=0){
					config.map.npc_array[i].$$$=0; 
					memset(&config.map.npc_array[i],0,sizeof(npc));	
//					printf("del npc\n");							
				}
				if (checkNpcTexWalk(config.map.npc_array[i].current_tex)){
//					config.map.npc_array[i].current_tex=getWalkTex(config.map.npc_array[i].direction);
					config.map.npc_array[i].position.x+=config.map.npc_array[i].direction.x;
					config.map.npc_array[i].position.y+=config.map.npc_array[i].direction.y;
				}
			}
		//tower
		for(i=0;i<config.map.tower_max;i++)
			if (config.map.tower_array[i].id!=0){
				if (config.map.tower_array[i].health<=0){// && 
				//		config.map.tower_array[i].anim_ended!=0){
					config.map.tower_array[i].$$$=0;
					config.map.grid[posToId(config.map.tower_array[i].position)].tower_id=0;
					memset(&config.map.tower_array[i],0,sizeof(tower));
//					printf("del tower\n");
					//change to set animation
				}
			}
		//bullet
		for(i=0;i<config.map.bullet_max;i++)
			if(config.map.bullet_array[i].id!=0){
				config.map.bullet_array[i].position.x+=config.map.bullet_array[i].direction.x;
				config.map.bullet_array[i].position.y+=config.map.bullet_array[i].direction.y;
				if (config.map.bullet_array[i].detonate!=0){// && 
				//		config.map.bullet_array[i].anim_ended!=0){
					config.map.bullet_array[i].id=0;
					memset(&config.map.bullet_array[i],0,sizeof(bullet));
//					printf("del bullet %d \n",config.map.bullet_array[i].anim_ended);
				}
			}
		//splash
		for(i=0;i<config.map.splash_max;i++)
			if(config.map.splash_array[i].type!=0){
				if (config.map.splash_array[i].anim_ended!=0){
					config.map.splash_array[i].type=0;
					memset(&config.map.splash_array[i],0,sizeof(splash));
				}
			}
		
	}
	printf("exit workerMap\n");
	return 0;
}

SDL_Thread* workerMapStart(){
	worker_arg arg;
	printf("start worker....\n");
	return SDL_CreateThread(workerMap, "WorkerMap", (void*)&arg);
}

int connectorMap(void *ptr){
//	worker_arg * arg=ptr;
	SDLNet_SocketSet socketset;
	socketset = SDLNet_AllocSocketSet(1); 
	if (SDLNet_TCP_AddSocket(socketset, config.map.network.socket)<0)
		perror("add to sockset");
		//auth
//	SDL_Delay(900);
	printf("done\n");
	while(config.map.enable){
	//	config.map.time_now=SDL_GetTicks();
		if(SDLNet_CheckSockets(socketset, 10)!=0)
		//get data from server
			if(recvMesMap()<0){
				perror("network error");
				setScreenMessage("network error");
				config.map.enable=0;
			}
	}
	SDLNet_FreeSocketSet(socketset);
	//try to read from public
	if (config.map.network.socket!=0){
		SDLNet_TCP_Close(config.map.network.socket);
		config.map.network.socket=0;
	}
	printf("exit connectorMap\n");
	return 0;
}

SDL_Thread* connectorMapStart(){
	worker_arg arg;
	printf("start connector....\n");
	return SDL_CreateThread(connectorMap, "ConnectorMap", (void*)&arg);
}


int drawerThread(void *ptr){
//	worker_arg * arg=ptr;
//	TCPsocket sock=config.map.network.socket;
	Uint32 time=0;
	
	SDL_GLContext glcontext = SDL_GL_CreateContext(config.window); // создаем контекст OpenGL
	if(glcontext == NULL){
		exit(1);
	}
/*
	GLint  iMultiSample = 0;
	GLint  iNumSamples = 0;
	glGetIntegerv(GL_SAMPLE_BUFFERS_ARB, &iMultiSample);
	glGetIntegerv(GL_SAMPLES_ARB, &iNumSamples);
	printf("Multisample buffers %d samples %d\n",iMultiSample,iNumSamples);
*/	
	int tmp[2]={0,0};
//	glGetIntegerv(GL_MAX_VIEWPORT_DIMS,tmp);
//	printf("\t%d %d\n",tmp[0],tmp[1]);
	SDL_GL_GetDrawableSize(config.window,&tmp[0],&tmp[1]);
	config.options.window.width=tmp[0];
	config.options.window.height=tmp[1];
	
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_MULTISAMPLE_ARB);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,config.options.window.width,0,config.options.window.height,-10000,10000);
	glMatrixMode(GL_MODELVIEW); 
	glRInit();
//	glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);//GL_NICEST
	
	////////////load global textures
	loadTexture(&cursor.tex,"global/cursor");
	loadTexture(&config.map.tex[ERROR_T],"global/error");
	loadTexture(&config.map.tex[BUILDABLE],"global/build");
	loadTexture(&config.map.tex[WALKABLE],"global/walk");
	loadTexture(&config.map.tex[NO_SEE],"global/see");
	loadTexture(&config.map.tex[LIGHT],"global/light_mask");
	loadTexture(&config.map.tex[DARKNESS],"global/darkness");
	loadTexture(&config.map.tex[HEALTH],"global/health");
	loadTexture(&config.map.tex[HEALTH_PAPER],"global/health_paper");
	loadTexture(&config.map.tex[TOWER_MARK],"global/tower_mark");
	loadTexture(&config.map.tex[CURRENT_NODE],"global/current_node");
	loadTexture(&config.map.tex[REMOVE_OBJECT],"global/remove_object");
	initLights();
	loadTexture(&config.map.minimap.tex,"global/minimap");
	
	/////////////tex load must be only here
	
	printf("done\n");
	while(config.main_running){ 
//	while(1){ 
		frameSync(&time);
		config.global.screen.coord.ld.x=screenToGridX(SCREEN_OFFSET,SCREEN_OFFSET);
		config.global.screen.coord.ld.y=screenToGridY(SCREEN_OFFSET,SCREEN_OFFSET);
		config.global.screen.coord.lu.x=screenToGridX(SCREEN_OFFSET,config.options.window.height-SCREEN_OFFSET);
		config.global.screen.coord.lu.y=screenToGridY(SCREEN_OFFSET,config.options.window.height-SCREEN_OFFSET);
		config.global.screen.coord.ru.x=screenToGridX(config.options.window.width-SCREEN_OFFSET,config.options.window.height-SCREEN_OFFSET);
		config.global.screen.coord.ru.y=screenToGridY(config.options.window.width-SCREEN_OFFSET,config.options.window.height-SCREEN_OFFSET);
		config.global.screen.coord.rd.x=screenToGridX(config.options.window.width-SCREEN_OFFSET,SCREEN_OFFSET);
		config.global.screen.coord.rd.y=screenToGridY(config.options.window.width-SCREEN_OFFSET,SCREEN_OFFSET);
		
		config.global.screen.node.l=getGridLineX(config.global.screen.coord.ld.x,config.global.screen.coord.ld.y)-3;
		config.global.screen.node.r=getGridLineX(config.global.screen.coord.rd.x,config.global.screen.coord.rd.y)+3;
		config.global.screen.node.d=getGridLineY(config.global.screen.coord.ld.x,config.global.screen.coord.ld.y)-3;
		config.global.screen.node.u=getGridLineY(config.global.screen.coord.lu.x,config.global.screen.coord.lu.y)+3;
		
		drawScene();
		if (config.map.clean_textures_size!=0){
			glDeleteTextures(config.map.clean_textures_size,(unsigned int*)config.map.clean_textures);
			config.map.clean_textures_size=0;
			memset(config.map.tex+COMON_TEXTURES_START,0,sizeof(config.map.tex)-sizeof(texture)*COMON_TEXTURES_START);
		}
		
	}
//	glFontDestroy(&mainfont);
	glDeleteTextures (config.textures_size,(unsigned int*)config.textures);
	config.textures_size=0;
	SDL_GL_DeleteContext(glcontext);
	
//	SDL_Delay(300);
	
	printf("exit drawer\n");
	SDL_Delay(800);
	return 0;
}

SDL_Thread* drawerStart(){
	worker_arg arg;
	printf("start drawer....\n");
	return SDL_CreateThread(drawerThread, "Drawer", (void*)&arg);
}
