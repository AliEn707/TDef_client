#include "headers.h"


/*
╔══════════════════════════════════════════════════════════════╗
║ 										                       ║
║ created by Dennis Yarikov						                       ║
║ nov 2014									                       ║
╚══════════════════════════════════════════════════════════════╝
*/

#define ELEMENT_SIZE 40

event * eventsCreateEvent(){
	event * u_u;
	object * n_n;
	element * e_e;
	color3 $_$={1,1,1,1};
	if ((u_u=malloc(sizeof(event)))==0)
		return 0;
	if ((n_n=malloc(sizeof(object)))==0){
		free(u_u);
		return 0;
	}
	memset(u_u,0,sizeof(event));
	u_u->o=n_n;
	memset(n_n,0,sizeof(object));
	n_n->$elements=1;
	if ((e_e=malloc(sizeof(element)))==0){
		free(u_u);
		free(n_n);
		return 0;
	}
	n_n->elements=e_e;
	
	n_n->action=actionEventChoose;
	n_n->focus=1;
	n_n->touch=1;
	memset(e_e,0,sizeof(element));
	memcpy(&e_e->color,&$_$,sizeof(color3));
	memcpy(&e_e->fcolor,&$_$,sizeof(color3));
	e_e->size.x=ELEMENT_SIZE;
	e_e->size.y=ELEMENT_SIZE;
		
	return u_u;
}

#define OFFSET 4
#define setcolor(x,R,G,B,A) x.color.r=R;x.color.g=G;x.color.b=B;x.color.a=A
#define setfcolor(x,R,G,B,A) x.fcolor.r=R;x.fcolor.g=G;x.fcolor.b=B;x.fcolor.a=A

void eventsFillMapMenu(){
	int left_border,info_width;
	menu* m_m=&config.public.map_info;
	static element e_e[1];
	static object o_o[3];
	memset(e_e,0,sizeof(e_e));
	memset(o_o,0,sizeof(o_o));
	//left down corner of map
	config.public._map_info.position=SCREEN_OFFSET+MAP_OFFSET+OFFSET;
	//size of map
	config.public._map_info.size=config.options.window.height-2*(SCREEN_OFFSET+MAP_OFFSET+OFFSET);
	printf("map position %d size %d\n",config.public._map_info.position,config.public._map_info.size);
	//load world map
	
	//left border of info column
	left_border=SCREEN_OFFSET+MAP_OFFSET+config.options.window.height-2*(SCREEN_OFFSET+MAP_OFFSET);
	//width of info column
	info_width=(config.options.window.width-(SCREEN_OFFSET+MAP_OFFSET))-left_border;
	//under map
	
	o_o[0].elements=&e_e[0];
	o_o[0].$elements=1; 
	o_o[0].elements[0].tex.frames=-1;
	o_o[0].position.x=left_border+OFFSET;
	o_o[0].position.y=config.options.window.height/2.0;//left_border+OFFSET;
	
	config.public._map_info.$rooms=o_o[0].elements[0].text;
	//test
	sprintf(config.public._map_info.$rooms,"%d",123);
	setcolor(o_o[0].elements[0],1,1,1,1);
	
//	o_o[0].position.x=SCREEN_OFFSET+MAP_OFFSET;
//	o_o[0].position.y=SCREEN_OFFSET+MAP_OFFSET;
	
//	o_o[0].elements[0].size.y=config.options.window.height-2*(SCREEN_OFFSET+MAP_OFFSET);
//	o_o[0].elements[0].size.x=o_o[0].elements[0].size.y;
	//config.options.window.width-2*(SCREEN_OFFSET+MAP_OFFSET);
	setcolor(o_o[0].elements[0],1,1,1,1);
	
	
	//menu setup
	m_m->objects=o_o;
	m_m->$objects=1;
	
	printf("set mapinfo menu\n");
}

int eventsFreeEvent(event* e) {
	free(e->o->elements);
	free(e->o);
	free(e);
	return 0;
}


//actions 

