#include "main.h"
#include "network.h"

#define checkMask(x,y) x&y

#define recvMap(x) recvData(config.map.network.socket,&x,sizeof(x))

void networkConnMap(char * addr,int port){
	config.map.network.socket=networkConn(addr,port);
}


void recvNpcMap(){
	npc * n;
	int id,bit_mask;
	recvMap(id);
	//find npc by id
	recvMap(bit_mask);
	if (checkMask(bit_mask,NPC_CREATE));
	if (checkMask(bit_mask,NPC_POSITION));
}



void getMessage(){
	
}
