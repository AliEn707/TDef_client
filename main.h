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
 
 #include "constants.h"
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
	int receive;
	float t_size;
	
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
	float t_size;
	
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
	
	char text[20];
	vec2 text_position;
	short text_centered;
	float text_size;
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
	int $elements;
	char text[20];
	int arg[4];
	void (*action)(void * arg);
} object;

typedef
struct menu{
	char enable;
	char map;
	int $objects;
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
	
	int money;	
	short status;
	//add hero info
} player_public;

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
	int map_$objects;
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
	g_params _transform;

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
	int enable;
	player_public player;
	netw network;
	
	menu lobby;  //searching room
	menu main;  //main controls
	menu map_under;  //when map on screen
	menu map_main;  //when map on screen
	menu map_info;  //when map on screen
	struct {
		int event;
		char* $rooms;
	} _map_info;
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
			int tex_size;
			char enable;
		} darkness;
		struct {
			float move_speed;
			float zoom_speed;
		} camera;
		struct {
			int elements;
		} wether;
		int tex_filter_mag;  //tex parameters
		int tex_filter_min;  //tex parameters
		float brightness;
		int tex_quality;
		color3 color;
		float tdf;
	} options; //params for options menu
}g_config;



typedef short char2;





#define sqr(a) ({typeof(a) $a=(a);($a)*($a);})

#define mapTex(a) ((int)((a)+COMON_TEXTURES_START))


#define idToX(id) (id/config.map.grid_size)
#define idToY(id) (id%config.map.grid_size)

#define getGridX(id) (1.0*idToX(id)+0.5f)
#define getGridY(id) (1.0*idToY(id)+0.5f)

//vetical and horisontal screen lines on grid
#define getGridLineX(x,y) ((int)(x)+(int)(y))
#define getGridLineY(x,y) (config.map.grid_size-(int)(x)+(int)(y))
//in screen check 
#define checkGridLines(x,y) ({int $x=getGridLineX(x,y), $y=getGridLineY(x,y);(config.global.screen.node.l<$x && config.global.screen.node.r>$x && config.global.screen.node.u>$y && config.global.screen.node.d<$y);})

#define posToId(v) (config.map.grid_size*((int)v.x)+((int)v.y))
#define cursor config.global.cursor


#define to2d(x,y)  ((x)*config.map.grid_size+(y))
#define to2di(x,y)  (((int)(x))*config.map.grid_size+((int)(y)))

#define ctoi(i) (i-48)


#define checkNpcTexAttack(x) (x==TEX_ATTACK_UP || x==TEX_ATTACK_DOWN || x==TEX_ATTACK_LEFT || x==TEX_ATTACK_LEFT_UP || x==TEX_ATTACK_LEFT_DOWN || x==TEX_ATTACK_RIGHT || x==TEX_ATTACK_RIGHT_UP || x==TEX_ATTACK_RIGHT_DOWN)
#define checkNpcTexWalk(x) (x==TEX_WALK_UP || x==TEX_WALK_DOWN || x==TEX_WALK_LEFT || x==TEX_WALK_LEFT_UP || x==TEX_WALK_LEFT_DOWN || x==TEX_WALK_RIGHT || x==TEX_WALK_RIGHT_UP || x==TEX_WALK_RIGHT_DOWN)

#define Color4f(red,green,blue,alpha) glColor4f((red)*config.options.brightness*config.options.color.r,(green)*config.options.brightness*config.options.color.g,(blue)*config.options.brightness*config.options.color.b,alpha)
#define Vertex2f(x,y) 1
#define Vertex3f(x,y,z) 1
#define TexCoord2f(x,y) 1

//#define recvData SDLNet_TCP_Recv

g_config config;
