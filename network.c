#include "main.h"
#include "network.h"

void networkConnMap(char * addr,int port){
	config.map.network.socket=networkConn(addr,port);
}

void recvNpcMap(){
	
}