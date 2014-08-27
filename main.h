#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <GL/gl.h>
#include <locale.h>
#include "glfont.h"
 
// screen frames per texture frames
#define FpF 5
//write to config
#define Df 0.26f

#define MAX_TEXTURES 10000
//texture global
#define MAX_TEX_GLOBAL 100


//textures map
#define MAP_COMON_TEXTURES_MAX 500
#define ERROR_T 1
#define WALKABLE 2
#define BUILDABLE 3
#define NO_SEE 4
#define COMON_TEXTURES_START 5

//process object
#define MOUSE 1
#define KEYBOARD 2


#define TEXTURES 20
#define TEXTURE_FRAMES 25

//textures npc
#define TEX_IDLE 0
#define TEX_DESTROY 1
#define TEX_WALK_LEFT 2
#define TEX_WALK_UP 3
#define TEX_WALK_DOWN 4
#define TEX_WALK_RIGHT 5
#define TEX_WALK_RIGHT_UP 6
#define TEX_WALK_RIGHT_DOWN 7
#define TEX_WALK_LEFT_UP 8
#define TEX_WALK_LEFT_DOWN 9
#define TEX_ATTACK 10
#define TEX_ATTACK_LEFT 11
#define TEX_ATTACK_RIGHT 12
#define TEX_ATTACK_UP 13
#define TEX_ATTACK_DOWN 14
#define TEX_ATTACK_RIGHT_UP 15
#define TEX_ATTACK_RIGHT_DOWN 16
#define TEX_ATTACK_LEFT_UP 17
#define TEX_ATTACK_LEFT_DOWN 18

//msg to client
#define MSG_TEST 0
#define MSG_NPC 1
#define MSG_TOWER 2
#define MSG_BULLET 3
#define MSG_PLAYER 4
//msg to server
#define MSG_SPAWN_TOWER 1
#define MSG_SPAWN_NPC 2


//bit mask
#define setMask(z,x) z->bit_mask|=x
//#define checkMask(z,x) z->bit_mask&x

#define PLAYER_HEALTH 1

#define NPC_HEALTH 1
#define NPC_POSITION 2
#define NPC_CREATE 4
#define NPC_LEVEL 8

#define TOWER_HEALTH 1
#define TOWER_TARGET 2
#define TOWER_CREATE 4
#define TOWER_LEVEL 8

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
	int tex[TEXTURE_FRAMES];
	int frames;
	short loop;
	short lf_delay;
	short lf_delay_counter;
	short fd_delay;
	short fd_delay_counter;
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
	short solid;
	
	char tex_path[TEXTURES][100];
	texture tex[TEXTURES];
}bullet_type;

typedef
struct tower{
	int id;
	vec2 position;
	short target;
	int type;
	int health;
	int shield;
	int energy;
	int owner;
	short level;
	
	int current_tex;
	texture tex[TEXTURES];
	short anim_ended;
	color3 color;
}tower;

typedef
struct npc{
	int id;
	char status;
	char group;
	vec2 position;
	vec2 destination;
	vec2 direction;
	int type;
	int health;
	int shield;
	short level;
	
	int current_tex;
	char attack_prepare;
	texture tex[TEXTURES];
	short anim_ended;
	color3 color;
	int prev_time;//try to use more then 1 tick
}npc;

typedef
struct bullet{
	int id;
	vec2 position;
	vec2 destination;
	vec2 direction;
	vec2 source;
	char group;
	char support;
	int type;
	char detonate;
	int owner;
	
	int current_tex;
	texture tex[TEXTURES];
	short anim_ended;
	int prev_time;
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
	texture ftex;
	short focus_tex;
	int wire;
	color3 color;
	color3 fcolor;
	char text[24];
	vec2 text_position;
	short text_centered;
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
	char text[24];
	int arg[4];
	void (*action)(void * arg);
} object;

typedef
struct menu{
	char enable;
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
	
	char * text;
} cur;


typedef
struct netw{
	TCPsocket socket;
	int err;
	char server[40];
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
	
	int time_now;
	
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
	SDL_Thread* connector;
	
	char text[30];
} map_conf;

typedef
struct global_conf{
	cur cursor;
	int keys[5000];
	int mouse[100];
	
	struct {
		GLFONT all;
	} font;
}global_conf;


typedef
struct public_conf{
	netw network;
}public_conf;

typedef
struct {
	short enable;
	short pos;
	char text[200];
	char ** text_edit;
} text_puts;

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
	
	text_puts text;
	
	short auth;
	menu auth_menu;
	
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
	
	struct {
		int tower_level;
	} player;
}g_config;









#define sqr(a) ((a)*(a))

#define mapTex(a) ((int)((a-'a')+COMON_TEXTURES_START))



#define posToId(v) (config.map.grid_size*((int)v.x)+((int)v.y))
#define cursor config.global.cursor


#define to2d(x,y)  ((x)*config.map.grid_size+(y))
#define to2di(x,y)  (((int)(x))*config.map.grid_size+((int)(y)))

#define ctoi(i) (i-48)


#define checkNpcTexAttack(x) (x==TEX_ATTACK_UP || x==TEX_ATTACK_DOWN || x==TEX_ATTACK_LEFT || x==TEX_ATTACK_LEFT_UP || x==TEX_ATTACK_LEFT_DOWN || x==TEX_ATTACK_RIGHT || x==TEX_ATTACK_RIGHT_UP || x==TEX_ATTACK_RIGHT_DOWN)
#define checkNpcTexWalk(x) (x==TEX_WALK_UP || x==TEX_WALK_DOWN || x==TEX_WALK_LEFT || x==TEX_WALK_LEFT_UP || x==TEX_WALK_LEFT_DOWN || x==TEX_WALK_RIGHT || x==TEX_WALK_RIGHT_UP || x==TEX_WALK_RIGHT_DOWN)

#define mainfont config.global.font.all


g_config config;