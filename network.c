#include "headers.h"

/*
╔══════════════════════════════════════════════════════════════╗
║ 										                       ║
║ created by Dennis Yarikov						                       ║
║ jul 2014									                       ║
╚══════════════════════════════════════════════════════════════╝
*/


#define checkMask(x,y) x&y

//check may be need to check <0
#define recvMap(x) if(recvData(config.map.network.socket,&x,sizeof(x))<=0) return -1


int recvData(TCPsocket sock, void * buf, int size){
	int need=size;
	int get;
	get=SDLNet_TCP_Recv(sock,buf,need);
	if (get<=0)
		return get;
	if (get==need)
		return get;
//	printf("get not all\n");
	need-=get;
	while(need>0){
		if((get=SDLNet_TCP_Recv(sock,buf+(size-need),need))<=0)
			return get;
		need-=get;
	}
	return size;
}


int networkInit(){
	if(SDLNet_Init()==-1)
	{
		printf("SDLNet_Init: %s\n",SDLNet_GetError());
		return -1;
	}
	return 0;
}

void networkExit(){
	return SDLNet_Quit();
}

TCPsocket networkConn(char * addr,int port){
	IPaddress ip;
	TCPsocket sock;
	printf("connect to server....");
	if(SDLNet_ResolveHost(&ip,addr,port)==-1)
	{
		printf("SDLNet_ResolveHost: %s\n",SDLNet_GetError());
		return 0;
	}
	if((sock=SDLNet_TCP_Open(&ip))==0){
		printf("fail\n");
		return 0;
	}
	printf("done\n");
	return sock;
}


int networkClose(TCPsocket sock){
	SDLNet_TCP_Close(sock);
	return 0;
}

TCPsocket networkConnMap(char * addr,int port){
	config.map.network.socket=networkConn(addr,port);
	return config.map.network.socket;
}


static inline int recvNpcMap(){
	npc * n;
	int id,bit_mask;
	npc tmp;
	
	recvMap(id);
	//find npc by id
	if ((n=getNpcById(id))==0){
		perror("getNpcById recvNpcMap");
		n=&tmp;
	}
	float shift;
//	if (n->prev_time==0)
		shift=config.global.latency;
	float add;
	int time_now=SDL_GetTicks();
	if (n->prev_time!=0)
		if ((add=(time_now-n->prev_time)/config.time_per_tick)>shift)
			shift+=add;
//	printf("%g %g\n",shift, add);
	n->prev_time=time_now;
	
	recvMap(bit_mask);
	if (checkMask(bit_mask,NPC_CREATE)){
//		printf("get new npc %d\n",n->type);
		recvMap(n->owner);
		recvMap(n->type);
//		recvMap(n->destination);
	}
//	if (checkMask(bit_mask,NPC_POSITION)){
	recvMap(n->destination);
	if (!checkNpcTexAttack(n->current_tex)){
		if (checkMask(bit_mask,NPC_CREATE))
			memcpy(&n->position,&n->destination,sizeof(vec2));
		vec2 dir={n->destination.x-n->position.x,n->destination.y-n->position.y};
		dir.x/=shift;
		dir.y/=shift;
		memcpy(&n->direction,&dir,sizeof(vec2));
		n->current_tex=getWalkTex(n->direction);
	}
//	}
	if(checkMask(bit_mask,NPC_LEVEL) || checkMask(bit_mask,NPC_CREATE)){
		recvMap(n->level);
	}
	if(checkMask(bit_mask,NPC_HEALTH) || checkMask(bit_mask,NPC_CREATE))
		recvMap(n->health);
	if (n->id==0 && checkMask(bit_mask,NPC_CREATE))
		n->id=id;
	return 0;
}


static inline int recvTowerMap(){
	tower * t;
	int id,bit_mask;
	tower tmp;
	
	recvMap(id);
	//find npc by id
	if((t=getTowerById(id))==0){
		perror("getTowerById recvTowerMap");
		t=&tmp;
	}
	recvMap(bit_mask);
	if (checkMask(bit_mask,TOWER_CREATE)){
//		printf("get new tower %d\n",t->type);		
		recvMap(t->type);
		recvMap(t->owner);
		int position;
		recvMap(position);
		//add
//		printf("egt tower on %d with %d\n",position,id);
		config.map.grid[position].tower_id=id;
//		printf("%d\n",config.map.grid[position].tower_id);
		t->position.x=position/config.map.grid_size+0.5;
		t->position.y=position%config.map.grid_size+0.5;
//		printf("tower on %g %g\n",t->position.x,t->position.y);
		
	}
	if (checkMask(bit_mask,TOWER_TARGET) || checkMask(bit_mask,TOWER_CREATE))
		recvMap(t->target);
	if(checkMask(bit_mask,TOWER_LEVEL) || checkMask(bit_mask,TOWER_CREATE))
		recvMap(t->level);
	if(checkMask(bit_mask,TOWER_HEALTH) || checkMask(bit_mask,TOWER_CREATE))
		recvMap(t->health);
	if (t->id==0 && checkMask(bit_mask,TOWER_CREATE))
		t->id=id;
	return 0;
}

static inline int recvBulletMap(){
	bullet * b;
	int id,bit_mask;
	bullet tmp;
	
	recvMap(id);
	//find npc by id
	if((b=getBulletById(id))==0){
		perror("getBulletById recvBulletMap");
		b=&tmp;
	}
	float shift;
//	if (b->prev_time==0)
		shift=config.global.latency;
	float add;
	int time_now=SDL_GetTicks();
	if (b->prev_time!=0)
		if ((add=(time_now-b->prev_time)/config.time_per_tick)>shift)
			shift+=add;
//	printf("%g\n",shift);
	b->prev_time=time_now;
	
	recvMap(bit_mask);
	recvMap(b->destination);
	if (checkMask(bit_mask,BULLET_CREATE))
		memcpy(&b->position,&b->destination,sizeof(vec2));
//	printf("\t%g %g\n",b->position.x,b->position.y);
	vec2 dir={b->destination.x-b->position.x,b->destination.y-b->position.y};
	dir.x/=shift;//15.0f;
	dir.y/=shift;//15.0f;
	memcpy(&b->direction,&dir,sizeof(vec2));
	if (checkMask(bit_mask,BULLET_CREATE)){
		npc* n;
		recvMap(b->type);
		recvMap(b->owner);
	
		n=getNpcById(b->owner);
		if (n!=0)
			if (n->id!=0){
	//			n->current_tex=TEX_ATTACK;
				n->attack_prepare++;
				vec2 dir={b->position.x-n->position.x,b->position.y-n->position.y};
				dir.x/=5;
				dir.y/=5;
				memcpy(&n->direction,&dir,sizeof(vec2));
				memcpy(&n->position,&n->destination,sizeof(vec2));
			}
		recvMap(b->source);
//		recvMap(b->destination);
	}
	//need to to correct solid, may be...
	bullet_type* type=typesBulletGet(b->type);
	if (type!=0)
		if (type->solid!=0)
			memset(&b->direction,0,sizeof(vec2));
	
	if(checkMask(bit_mask,BULLET_DETONATE)){
		recvMap(b->detonate);
		//add animation detonation set
	}
	if (b->id==0 && checkMask(bit_mask,BULLET_CREATE))
		b->id=id;
	return 0;
}

static inline int recvPlayerMap(){
	int id,bit_mask;
	recvMap(id);
	//check npc
	recvMap(bit_mask);
	if (checkMask(bit_mask,PLAYER_CREATE)){
		recvMap(config.map.players[id].id);
		recvMap(config.map.players[id].tower_set);
		recvMap(config.map.players[id].npc_set);
		recvMap(config.map.players[id].group);
	}
	if(checkMask(bit_mask,PLAYER_HEALTH) || checkMask(bit_mask,PLAYER_CREATE)){
		recvMap(config.map.players[id].base_type.health);
	}
	if(checkMask(bit_mask,PLAYER_MONEY)){
		recvMap(config.map.players[id].money);
		printf("player money %d \n",config.map.players[id].money);
	}
	return 0;
}

int recvMesMap(){
	char mes;
//	int err;
//	networkSetNonBlock(config.map.network.socket);
	recvMap(mes);
//	networkSetBlock(config.map.network.socket);
	if (mes==MSG_NPC){
//		printf("get npc\n");
		return recvNpcMap();
	}
	if (mes==MSG_TOWER){
//		printf("get tower\n");
		return recvTowerMap();
	}
	if (mes==MSG_BULLET){
//		printf("get bullet\n");
		return recvBulletMap();
	}
	if (mes==MSG_PLAYER){
//		printf("get player\n");
		return recvPlayerMap();
	}
	printf("unnown message\n");
	return 0;
}


void checkMapLatency(){
	int a,b;
	int mes=0;
	
	a=SDL_GetTicks();
	SDLNet_TCP_Send(config.map.network.socket,&mes,sizeof(mes));
	recvData(config.map.network.socket,&mes,sizeof(mes));
	b=SDL_GetTicks();
	printf("latency test %dms %d tiks\n",(b-a),(b-a)/config.time_per_tick);
	config.global.latency=7+((b-a)/config.time_per_tick);
	printf("latency set to %g\n",config.global.latency);
}

int networkMapAuth(){
	recvMap(config.map.player_id);
	config.map.player=&config.map.players[config.map.player_id];
	checkMapLatency();
	return 0;
}

