#include <math.h>

#include "main.h"
#include "network.h"
#include "engine.h"

#define checkMask(x,y) x&y

#define recvMap(x) if(SDLNet_TCP_Recv(config.map.network.socket,&x,sizeof(x))<0) return -1



int networkInit(){
	if(SDLNet_Init()==-1)
	{
		printf("SDLNet_Init: %s\n",SDLNet_GetError());
		return -1;
	}
}

void networkExit(){
	SDLNet_Quit();
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
}

TCPsocket networkConnMap(char * addr,int port){
	config.map.network.socket=networkConn(addr,port);
	return config.map.network.socket;
}


int recvNpcMap(){
	npc * n;
	int id,bit_mask;
	recvMap(id);
	//find npc by id
	if ((n=getNpcById(id))==0){
		perror("getNpcId recvNpcMap");
		return 0;
	}
	recvMap(bit_mask);
	if (checkMask(bit_mask,NPC_CREATE)){
		recvMap(n->isfriend);
		recvMap(n->type);
//		printf("%d\n",n->type);
//		recvMap(n->destination);
	}
//	if (checkMask(bit_mask,NPC_POSITION)){
	recvMap(n->destination);
	if (!checkNpcTexAttack(n->current_tex)){
		if (checkMask(bit_mask,NPC_CREATE))
			memcpy(&n->position,&n->destination,sizeof(vec2));
		vec2 dir={n->destination.x-n->position.x,n->destination.y-n->position.y};
		dir.x/=5.0f;
		dir.y/=5.0f;
		memcpy(&n->direction,&dir,sizeof(vec2));
		n->current_tex=getWalkTex(n->direction);
	}
//	}
	if(checkMask(bit_mask,NPC_HEALTH) || checkMask(bit_mask,NPC_CREATE))
		recvMap(n->health);
	if (n->id==0 && checkMask(bit_mask,NPC_CREATE))
		n->id=id;
	return 0;
}


int recvTowerMap(){
	tower * t;
	int id,bit_mask;
	recvMap(id);
	//find npc by id
	if((t=getTowerById(id))==0){
		perror("getTowerId recvTowerMap");
		return 0;
	}
	recvMap(bit_mask);
	if (checkMask(bit_mask,TOWER_CREATE)){
		recvMap(t->type);
		recvMap(t->owner);
		int position;
		recvMap(position);
		//add
		config.map.grid[position].tower_id=id;
		t->position.x=position/config.map.grid_size+0.5;
		t->position.y=position%config.map.grid_size+0.5;
		printf("tower on %g %g\n",t->position.x,t->position.y);
		
	}
	if (checkMask(bit_mask,TOWER_TARGET) || checkMask(bit_mask,TOWER_CREATE))
		recvMap(t->target);
	if(checkMask(bit_mask,TOWER_HEALTH) || checkMask(bit_mask,TOWER_CREATE))
		recvMap(t->health);
	if (t->id==0 && checkMask(bit_mask,TOWER_CREATE))
		t->id=id;
	return 0;
}

int recvBulletMap(){
	bullet * b;
	int id,bit_mask;
	recvMap(id);
	//find npc by id
	if((b=getBulletById(id))==0){
		perror("getBulletId recvBulletMap");
		return 0;
	}
	recvMap(bit_mask);
	recvMap(b->destination);
	if (checkMask(bit_mask,BULLET_CREATE))
		memcpy(&b->position,&b->destination,sizeof(vec2));
//	printf("\t%g %g\n",b->position.x,b->position.y);
	vec2 dir={b->destination.x-b->position.x,b->destination.y-b->position.y};
	dir.x/=7.7f;
	dir.y/=7.7f;
	memcpy(&b->direction,&dir,sizeof(vec2));
	if (checkMask(bit_mask,BULLET_CREATE)){
		npc* n;
		recvMap(b->type);
		recvMap(b->owner);
	
		n=getNpcById(b->owner);
		if (n->id!=0){
//			n->current_tex=TEX_ATTACK;
			n->attack_prepare++;
			vec2 dir={b->position.x-n->position.x,b->position.y-n->position.y};
			dir.x/=5;
			dir.y/=5;
			memcpy(&n->direction,&dir,sizeof(vec2));
		}
		recvMap(b->source);
//		recvMap(b->destination);
	}
	if(checkMask(bit_mask,BULLET_DETONATE)){
		recvMap(b->detonate);
		//add animation detonation set
	}
	if (b->id==0 && checkMask(bit_mask,BULLET_CREATE))
		b->id=id;
	return 0;
}

int recvMesMap(){
	char mes;
	int err;
//	networkSetNonBlock(config.map.network.socket);
	recvMap(mes);
//	networkSetBlock(config.map.network.socket);
	if (mes==MSG_NPC){
		return recvNpcMap();
	}
	if (mes==MSG_TOWER){
		return recvTowerMap();
	}
	if (mes==MSG_BULLET){
		return recvBulletMap();
	}
	printf("unnown message\n");
	return 0;
}
