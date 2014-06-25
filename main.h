#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>

#include "glfont.h"


typedef
struct vec2{
	float x;
	float y;
} vec2;

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
	vec2 position;
	vec2 size;
	element * elements;
	int elements_size;
	int arg[4];
} object;

typedef
struct menu{
	object * objects;
	struct menu * submenu;
} menu;

typedef
struct menuroot{
	char enable;
	int depth;
	int path[10];
	menu* submenu;
}menu_root;


typedef
struct g_config{
	g_params transform;
	unsigned int time_per_frame;
	unsigned int time_per_tick;
	SDL_Window *window;
	int window_width; 
	int window_height; 
	int main_running;
	menu_root menu;
	int textures_size;
	int textures[10000];
}g_config;























g_config config;