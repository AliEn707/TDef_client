#include "main.h"
#include "network.h"

#define checkMask(x,y) x&y

#define recvMap(x) if(recvData(config.map.network.socket,&x,sizeof(x))<0) return -1

int networkConnMap(char * addr,int port){
	config.map.network.socket=networkConn(addr,port);
}


npc* getNpcId(int id){
	int i;
	for(i=0;i<config.map.npc_max;i++)
		if (config.map.npc_array[i].id==id)
			return &config.map.npc_array[i];
}

tower* getTowerId(int id){
	int i;
	for(i=0;i<config.map.tower_max;i++)
		if (config.map.tower_array[i].id==id)
			return &config.map.tower_array[i];
}
int recvNpcMap(){
	npc * n;
	int id,bit_mask;
	recvMap(id);
	//find npc by id
	n=getNpcId(id);
	recvMap(bit_mask);
	if (checkMask(bit_mask,NPC_CREATE)){
		recvMap(n->isfriend);
		recvMap(n->type);
		recvMap(n->destination);
		memcpy(&n->position,&n->destination,sizeof(vec2));
	}
	if (checkMask(bit_mask,NPC_POSITION))
		recvMap(n->destination);
	if(checkMask(bit_mask,NPC_HEALTH) || checkMask(bit_mask,NPC_CREATE))
		recvMap(n->health);
}


int recvTowerMap(){
	tower * t;
	int id,bit_mask;
	recvMap(id);
	//find npc by id
	t=getTowerId(id);
	recvMap(bit_mask);
	if (checkMask(bit_mask,TOWER_CREATE)){
		recvMap(t->type);
		recvMap(t->owner);
		recvMap(t->position);
	}
	if (checkMask(bit_mask,TOWER_TARGET) || checkMask(bit_mask,TOWER_CREATE))
		recvMap(t->target);
	if(checkMask(bit_mask,TOWER_HEALTH) || checkMask(bit_mask,TOWER_CREATE))
		recvMap(t->health);
}



int recvMesMap(){
	char mes;
	int err;
	recvMap(mes);
	if (mes==MSG_NPC){
		return recvNpcMap();
	}
	if (mes==MSG_TOWER){
		return recvTowerMap();
	}
	
	return 0;
}
