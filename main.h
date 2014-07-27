#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
 
// screen frames per texture frames
#define FpF 5
//write to config
#define Df 0.29f

#define MAX_TEXTURES 10000
//texture global
#define MAX_TEX_GLOBAL 100

//textures map
#define MAP_COMON_TEXTURES_MAX 500
#define ERROR 0
#define WALKABLE 1
#define BUILDABLE 2
#define NO_SEE 3
#define COMON_TEXTURES_START 4

//process object
#define MOUSE 1
#define KEYBOARD 2


#define TEXTURES 20
//textures npc
#define TEX_IDLE 0
#define TEX_DESTROY 1

//bit mask
#define setMask(z,x) z->bit_mask|=x
//#define checkMask(z,x) z->bit_mask&x

#define MSG_TEST 0
#define MSG_NPC 1
#define MSG_TOWER 2
#define MSG_BULLET 3
#define MSG_PLAYER 4

#define PLAYER_HEALTH 1

#define NPC_POSITION 1
#define NPC_HEALTH 2
#define NPC_CREATE 4

#define TOWER_HEALTH 1
#define TOWER_TARGET 2
#define TOWER_CREATE 4

#define BULLET_POSITION 1
#define BULLET_DETONATE 2
#define BULLET_CREATE 4




typedef
struct vec2{
	float x;
	float y;
} vec2;

typedef
struct vec2i{
	int x;
	int y;
} vec2i;

typedef
struct color3{
	float r;
	float g;
	float b;
	float a;
} color3;

typedef 
struct texture{
	int tex[25];
	int frames;
	float current_frame;
} texture;


//////////

typedef 
struct effect{ //модификаторы свойств > 0
	float speed;
	float shield;
	float damage;
	int time;
	int status; //вероятность срабатывания в %
} effect;

typedef
struct tower_type{
	int id;
	int health;
	int damage;
	int energy;
	int shield;
	int distanse;
	int attack_speed; //ticks to attack
	int cost;
	int ignor_type;
	int prior_type;
	int bullet_type;
	int support;
	
	char tex_path[TEXTURES][100];
	texture tex[TEXTURES];
//	effect effects;   //наносимые эффекты
}tower_type;

typedef
struct npc_type{
	int id;
	int health;
	int damage;
	int shield;
	int attack_distanse;
	int see_distanse;
	int attack_speed;
	float move_speed;
	int cost;
	int type;
	int ignor_type;
	int prior_type;
	int bullet_type;
	int support;
	int receive;
	
	char tex_path[TEXTURES][100];
	texture tex[TEXTURES];
//	effect effects;  //наносимые эффекты
}npc_type;

typedef
struct bullet_type{
	int id;
	float speed;
	int attack_type;
	int move_type;
	int area;
	
	char tex_path[TEXTURES][100];
	texture tex[TEXTURES];
}bullet_type;

typedef
struct tower{
	int id;
	int bit_mask;
	vec2 position;
	int target;
	int type;
	int health;
	int shield;
	int energy;
	int owner;
	
	int current_tex;
	texture tex[TEXTURES];
	color3 color;
}tower;

typedef
struct npc{
	int id;
	char status;
	char isfriend;
	vec2 position;
	vec2 destination;
	vec2 direction;
	int type;
	int health;
	int shield;
	
	int current_tex;
	texture tex[TEXTURES];
	short anim_ended;
	color3 color;
}npc;

typedef
struct bullet{
	int id;
	vec2 position;
	vec2 destination;
	vec2 direction;
	vec2 source;
	char isfriend;
	char support;
	char type;
	int detonate;
	int owner;
	
	int current_tex;
	texture tex[TEXTURES];
}bullet;

/////////////
typedef 
struct g_params{
	float scale;
	int scale_push;
	vec2 translate;
} g_params;

typedef
struct element{
	vec2 position;
	vec2 size;
	texture tex;
	int wire;
	color3 color;
	color3 fcolor;
} element;
	
typedef
struct object{
	int type;
	int focus;
	int in_focus;
	int touch;
	int single;
	int hide;
	vec2 position;
	element * elements;
	int elements_size;
	char text[25];
	int arg[4];
	void (*action)(void * arg);
} object;

typedef
struct menu{
	int objects_size;
	int submenu_size;
	object * objects;
	struct menu * submenu;
} menu;

typedef
struct gnode{
	int id;
	int tex;
	int walkable;
	int buildable;
	int tower_id;
	texture blood;
} gnode;

typedef 
struct cur{
	vec2 state;
	vec2i pos;
	vec2i prev;
	color3 color;
	texture tex;
	float sens;
} cur;


typedef
struct netw{
	int socket;
	int err;
} netw;

typedef
struct menu_conf{
	int enable;
	int depth;
	int path[10];
	menu root;
	object* selected;
}menu_conf;

typedef
struct map_conf{
	int enable;
	int grid_size;
	int focus;
	int show_walk;
	gnode * grid;
	gnode * grid_out[4]; //non working map zone 
	texture tex[MAP_COMON_TEXTURES_MAX];
	g_params transform;
	menu screen_menu;
	menu action_menu;
	menu npc_menu;
	int tower_max;
	tower * tower_array;
	int npc_max;
	npc * npc_array;
	int bullet_max;
	bullet * bullet_array;
	int textures_size;
	int textures[MAX_TEXTURES];
	netw network;
	SDL_Thread* worker;
} map_conf;

typedef
struct global_conf{
	cur cursor;
	int keys[5000];
	int mouse[100];
}global_conf;


typedef
struct public_conf{
	netw network;
}public_conf;


typedef
struct g_config{
	unsigned int time_per_frame;
	unsigned int time_per_tick;
	SDL_Window *window;
	int window_width; 
	int window_height; 
	int main_running;
	int texture_no_change;
	
	global_conf global;
	menu_conf menu;
	map_conf map;
	public_conf public;
	
	int textures_size;
	int textures[MAX_TEXTURES];
	int global_tex[MAX_TEX_GLOBAL];
	int global_count;
	
	unsigned int tower_types_size;
		tower_type* tower_types;
	unsigned int npc_types_size;
		npc_type* npc_types;
	unsigned int bullet_types_size;
		bullet_type* bullet_types;
	
}g_config;









#define sqr(a) ((a)*(a))

#define mapTex(a) ((int)((a-'a')+COMON_TEXTURES_START))



#define cursor config.global.cursor


#define to2d(x,y)  ((x)*config.map.grid_size+(y))
#define to2di(x,y)  (((int)(x))*config.map.grid_size+((int)(y)))

#define ctoi(i) (i-48)




g_config config;