#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
 



#define MAX_TEXTURES 10000
//texture global
#define MAX_TEX_GLOBAL 10
#define CURSOR 0
#define WALKABLE 1
#define BUILDABLE 2


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
} color3;

typedef 
struct g_params{
	float scale;
	vec2 translate;
} g_params;

typedef
struct element{
	vec2 position;
	vec2 size;
	int tex;
	char wire;
	color3 color;
} element;
	
typedef
struct object{
	char type;
	vec2 position;
	vec2 size;
	element * elements;
	int elements_size;
	int arg[4];
	void (*action)(void * arg);
} object;

typedef
struct menu{
	object * objects;
	struct menu * submenu;
} menu;

typedef
struct menu_conf{
	char enable;
	int depth;
	int path[10];
	menu* root;
	object background;
}menu_conf;

typedef
struct gnode{
	int id;
	int tex;
	char walkable;
	char buildable;
	int tower_id;
} gnode;


typedef
struct map_conf{
	int gridsize;
	gnode * grid;
} map_conf;

typedef 
struct cur{
	vec2 state;
	vec2i pos;
	vec2i prev;
	color3 color;
	float sens;
} cur;
typedef
struct global_conf{
	cur cursor;
}global_conf;

typedef
struct g_config{
	g_params transform;
	global_conf global;
	unsigned int time_per_frame;
	unsigned int time_per_tick;
	SDL_Window *window;
	int window_width; 
	int window_height; 
	int main_running;
	
	menu_conf menu;
	map_conf map;
	
	int textures_size;
	int textures[MAX_TEXTURES];
	int global_tex[MAX_TEX_GLOBAL];
}g_config;














#define cursor config.global.cursor








g_config config;