#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
 



#define MAX_TEXTURES 10000
//texture global
#define MAX_TEX_GLOBAL 100
#define CURSOR 0
#define WALKABLE 1
#define BUILDABLE 2

//process object
#define MOUSE 1
#define KEYBOARD 2

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

//////////
typedef
struct tower{
	int id;
	int bit_mask;
	vec2 position;
	int type;
	int health;
	int shield;
	int energy;
	
}tower;

typedef
struct npc{
	int id;
	int status;
	vec2 position;
	vec2 destination;
	vec2 direction;
	int type;
	int health;
	int shield;
	
}npc;

typedef
struct bullet{
	int id;
	vec2 position;
	vec2 destination;
	vec2 direction;
	vec2 source;
	int detonate;
}bullet;

/////////////
typedef 
struct g_params{
	float scale;
	int scale_push;
	vec2 translate;
} g_params;

typedef 
struct texture{
	int tex[25];
	int frames;
} texture;

typedef
struct element{
	vec2 position;
	vec2 size;
	int tex;
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
} gnode;

typedef 
struct cur{
	vec2 state;
	vec2i pos;
	vec2i prev;
	color3 color;
	float sens;
} cur;

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
	gnode * grid;
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
	
} map_conf;

typedef
struct global_conf{
	cur cursor;
	int keys[5000];
	int mouse[100];
}global_conf;

typedef
struct g_config{
	unsigned int time_per_frame;
	unsigned int time_per_tick;
	SDL_Window *window;
	int window_width; 
	int window_height; 
	int main_running;
	
	global_conf global;
	menu_conf menu;
	map_conf map;
	
	int textures_size;
	int textures[MAX_TEXTURES];
	int global_tex[MAX_TEX_GLOBAL];
	int global_count;
}g_config;














#define cursor config.global.cursor


#define to2d(x,y)  ((x)*config.map.grid_size+(y))
#define to2di(x,y)  (((int)(x))*config.map.grid_size+((int)(y)))

#define ctoi(i) (i-48)




g_config config;