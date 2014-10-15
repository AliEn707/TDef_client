#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
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
#define Df config.options.tdf//0.26f
#define TEX_FPS 13.0f
//base
#define BASE 0

#define MAX_TEXTURES 10000
//texture global
#define MAX_TEX_GLOBAL 100

//global drawing
#define SCREEN_OFFSET 20

//minimap
#define MINIMAP_SIZE 190
#define MINIMAP_AREA_WIDTH (MINIMAP_SIZE*1.41)
#define MINIMAP_AREA_HEIGHT (MINIMAP_AREA_WIDTH/2)
#define MINIMAP_AREA_SHIFT 12

//textures map
#define MAP_COMON_TEXTURES_MAX 500
#define ERROR_T 1
#define WALKABLE 2
#define BUILDABLE 3
#define NO_SEE 4
#define LIGHT 5
#define COMON_TEXTURES_START 6

//process object
#define MOUSE 1
#define KEYBOARD 2


#define TEXTURES 20
#define TEXTURE_FRAMES 25

#define BASE_TEX_LAYERS 10

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
#define PLAYER_CREATE 4

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

//global controls
#define CURSOR_SPEED cursor.speed
#define CAMERA_SPEED config.options.camera.move_speed
#define CAMERA_ZOOM config.options.camera.zoom_speed

//in game
#define NPC_SET_SIZE 9
#define TOWER_SET_SIZE 9

//brush
#define BRUSH_TOWER_SPAWN 1

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
	int owner;
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


typedef 
struct {
	int id;
	char tex_path[100];
	texture tex;
} splash_type;

typedef 
struct {
	vec2 position;
	short type;
	short anim_ended;
	texture tex;
} splash;



typedef 
struct {
	int position;
	char direction;
	
	int tex;
} wall;


typedef 
struct {
	vec2 position;
	
	int tex;
} map_object;



/////////////
typedef 
struct g_params{
	float scale;
	int scale_push;
	vec2 translate;
} g_params;

typedef
struct element{
	short map;
	short focus_tex;
	int wire;
	vec2 _position;
	float anim_speed;
	vec2 tile_size;
	
	vec2 position;
	vec2 size;
	texture tex;
	char tex_path[30];
	color3 color;
	color3 wirecolor;
	
	vec2 fposition;
	texture ftex;
	char ftex_path[30];
	color3 fcolor;
	color3 fwirecolor;
	
	char text[24];
	vec2 text_position;
	short text_centered;
	short text_size;
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
	char map;
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
} gnode;

typedef 
struct cur{
	vec2 state;
	vec2i pos;
	vec2i prev;
	color3 color;
	texture tex;
	float sens;
	float speed;
	
	char * text;
} cur;


typedef
struct netw{
	TCPsocket socket;
	int err;
	char server[40];
	int port;
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
struct {
	char tex_path[BASE_TEX_LAYERS][100];
	texture tex[BASE_TEX_LAYERS];
	
} base;


typedef
struct {
	struct {
		int id;
		int size;
	} tower_set[NPC_SET_SIZE];
	
	struct {
		int id;
		int size;
	} npc_set[TOWER_SET_SIZE];
	
	int id;
	int level;
	int group;

	int money;	
	int base_health;
	tower * base;
	//add hero info
} player;

typedef
struct map_conf{
	int enable;
	int focus;
	int show_walk;
	int grid_size;
	gnode * grid;
	gnode * grid_out[4]; //non working map zone 
	texture tex[MAP_COMON_TEXTURES_MAX];
	char tex_path[MAP_COMON_TEXTURES_MAX][100];
	
	menu screen_menu;
	menu action_menu;
	menu npc_menu;
	menu tower_menu;
	
	int time_now;
	
	int tower_max;
	tower * tower_array;
	int npc_max;
	npc * npc_array;
	int bullet_max;
	bullet * bullet_array;
	int splash_max;
	splash * splash_array;
	
	int textures_size;
	int textures[MAX_TEXTURES];
	
	int clean_textures_size;
	int clean_textures[MAX_TEXTURES];
	
	int walls_size;
	wall * walls;
	int map_objects_size;
	map_object* map_objects;
	
	netw network;
	SDL_Thread* worker;
	SDL_Thread* connector;
	
	char text[30];
	struct {
		short enable;
		short used;
		vec2 position;
		texture tex;
		object obj;
	} minimap;
	
	struct {
		short type;
		short id;
		void (*action)(void);
	} brush;
	
	player * player;
	int player_id;
	player players[20];
	object * tower_objects[TOWER_SET_SIZE];
	object * npc_objects[NPC_SET_SIZE];
} map_conf;

typedef
struct global_conf{
	float latency;
	cur cursor;
	int keys[5000];
	int mouse[100];
	g_params transform;

	float frame_time;
	struct {
		struct {
			int l;
			int r;
			int u;
			int d;
		} node; //grid lines
		
		struct {
			vec2 ld;
			vec2 lu;
			vec2 ru;
			vec2 rd;
		} coord; //grid coordinates of screen angles
	} screen;
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
	char * text_edit;
	menu keyboard;
} text_puts;

typedef
struct g_config{
	unsigned int time_per_frame;
	unsigned int time_per_tick;
	SDL_Window *window;

	int main_running;
	int texture_no_change;
	
	global_conf global;
	menu_conf menu;
	map_conf map;
	public_conf public;
	
	text_puts text;
	
	short auth;
	menu auth_menu;
	menu loading;
	
	char message[500];
	short message_ticks;
	
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
	unsigned int splash_types_size;
		splash_type* splash_types;
		
	
	SDL_Thread* drawer;
	
	struct {
		struct {
			int width; 
			int height; 
		} window;
		struct {
			int tex_width;
			int tex_height;
		} lights;
		struct {
			float move_speed;
			float zoom_speed;
		} camera;
		struct {
			int elements;
		} wether;
		float darkness;
		int tex_filter;  //tex parameters
		float brightness;
		color3 color;
		float tdf;
	} options; //params for options menu
}g_config;



typedef short char2;





#define sqr(a) ((a)*(a))

#define mapTex(a) ((int)((a)+COMON_TEXTURES_START))


#define idToX(id) (id/config.map.grid_size)
#define idToY(id) (id%config.map.grid_size)

#define getGridX(id) (1.0*idToX(id)+0.5f)
#define getGridY(id) (1.0*idToY(id)+0.5f)

//vetical and horisontal screen lines on grid
#define getGridLineX(x,y) ((int)(x)+(int)(y))
#define getGridLineY(x,y) (config.map.grid_size-(int)(x)+(int)(y))
//in screen check 
#define checkGridLines(x,y) (config.global.screen.node.l<getGridLineX(x,y) && config.global.screen.node.r>getGridLineX(x,y) && config.global.screen.node.u>getGridLineY(x,y) && config.global.screen.node.d<getGridLineY(x,y))

#define posToId(v) (config.map.grid_size*((int)v.x)+((int)v.y))
#define cursor config.global.cursor


#define to2d(x,y)  ((x)*config.map.grid_size+(y))
#define to2di(x,y)  (((int)(x))*config.map.grid_size+((int)(y)))

#define ctoi(i) (i-48)


#define checkNpcTexAttack(x) (x==TEX_ATTACK_UP || x==TEX_ATTACK_DOWN || x==TEX_ATTACK_LEFT || x==TEX_ATTACK_LEFT_UP || x==TEX_ATTACK_LEFT_DOWN || x==TEX_ATTACK_RIGHT || x==TEX_ATTACK_RIGHT_UP || x==TEX_ATTACK_RIGHT_DOWN)
#define checkNpcTexWalk(x) (x==TEX_WALK_UP || x==TEX_WALK_DOWN || x==TEX_WALK_LEFT || x==TEX_WALK_LEFT_UP || x==TEX_WALK_LEFT_DOWN || x==TEX_WALK_RIGHT || x==TEX_WALK_RIGHT_UP || x==TEX_WALK_RIGHT_DOWN)

#define Color4f(red,green,blue,alpha) glColor4f((red)*config.options.brightness*config.options.color.r,(green)*config.options.brightness*config.options.color.g,(blue)*config.options.brightness*config.options.color.b,alpha)

g_config config;
