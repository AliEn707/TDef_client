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
struct g_params{
	float scale;
	vec2 translate;
} g_params;


typedef
struct g_config{
	g_params transform;
	unsigned int time_per_frame;
	unsigned int time_per_tick;
	SDL_Window *window;
	int window_width; 
	int window_height; 
	int main_running;
}g_config;























g_config config;