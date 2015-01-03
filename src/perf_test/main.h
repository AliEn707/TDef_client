#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <strings.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_mixer.h>
#include <GL/gl.h>
#include <time.h> 
#include "../constants.h" 
#include "../main.h"
#include "../network.h"
#include "../map.h"
#include "../menu.h"
#include "../file.h"
#define sendData SDLNet_TCP_Send


typedef
struct {
	
	struct {
		TCPsocket sock;
		int latency;
	} player[4];
} room;

struct {
	int main_running;
	int global_count;
	
	int $servers;
	int time_per_serv;
	int latency;
	
	TCPsocket sock;
	char server[50];
	int port; //port of fake public
} perf_test;


#define sqr(a) ({typeof(a) $a=(a);($a)*($a);})
#define max(a,b) ({typeof(a) $a=(a);typeof(b) $b=(b);$a<$b?$b:$a;})

#define mapTex(a) ((int)((a)+COMON_TEXTURES_START))


#define idToX(id) (id/config.map.grid_size)
#define idToY(id) (id%config.map.grid_size)

#define getGridX(id) (1.0*idToX(id)+0.5f)
#define getGridY(id) (1.0*idToY(id)+0.5f)

//vetical and horisontal screen lines on grid
#define getGridLineX(x,y) ((x)+(y)) //CHECK: maybe need int
#define getGridLineY(x,y) (config.map.grid_size-(x)+(y)) //CHECK: maybe need int
//in screen check 
#define checkGridLines(x,y) ({float $x=getGridLineX(x,y), $y=getGridLineY(x,y);(config.global.screen.node.l<$x && config.global.screen.node.r>$x && config.global.screen.node.u>$y && config.global.screen.node.d<$y);})

#define posToId(v) (config.map.grid_size*((int)v.x)+((int)v.y))
#define cursor config.global.cursor


#define to2d(x,y)  ((x)*config.map.grid_size+(y)) 
#define to2di(x,y)  (((int)(x))*config.map.grid_size+((int)(y))) //

#define ctoi(i) (i-48)


#define checkNpcTexAttack(x) (x==TEX_ATTACK_UP || x==TEX_ATTACK_DOWN || x==TEX_ATTACK_LEFT || x==TEX_ATTACK_LEFT_UP || x==TEX_ATTACK_LEFT_DOWN || x==TEX_ATTACK_RIGHT || x==TEX_ATTACK_RIGHT_UP || x==TEX_ATTACK_RIGHT_DOWN)
#define checkNpcTexWalk(x) (x==TEX_WALK_UP || x==TEX_WALK_DOWN || x==TEX_WALK_LEFT || x==TEX_WALK_LEFT_UP || x==TEX_WALK_LEFT_DOWN || x==TEX_WALK_RIGHT || x==TEX_WALK_RIGHT_UP || x==TEX_WALK_RIGHT_DOWN)



//#define recvData SDLNet_TCP_Recv
