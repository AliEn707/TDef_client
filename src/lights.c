#include "headers.h"
/*
╔══════════════════════════════════════════════════════════════╗
║ 										                       ║
║ created by Dennis Yarikov						                       ║
║ sep 2014									                       ║
╚══════════════════════════════════════════════════════════════╝
*/

//int config.options.lights.tex_width=64;//config.options.window.width;
//int config.options.lights.tex_height=64;//config.options.window.height;
static char *mem;

static texture * lights;

void initLights(){
	//move to configuration
//	if (config.options.window.width<900){
//		config.options.lights.tex_width=32;
//		config.options.lights.tex_height=32;
//	} else {
//		config.options.lights.tex_width=64;
//		config.options.lights.tex_height=64;
//	}
	
	if ((mem=malloc(config.options.darkness.tex_size*config.options.darkness.tex_size*sizeof(char)))==0)
		perror("malloc initLights");
	if ((lights=malloc(sizeof(texture)))==0)
		perror("malloc initLights");
	
	memset(lights,0,sizeof(texture));
	
	createLightsTexture();
}

void realizeLights(){
	free(mem);
	free(lights);
}

int createLightsTexture(){
	unsigned int tex;

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	setTexture(0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, config.options.darkness.tex_size, config.options.darkness.tex_size, 0, GL_RGB, GL_UNSIGNED_INT, 0);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	lights->frames=1;
	lights->loop=1;
	lights->tex[0]=tex;

//	config.map.textures[config.map.textures_size++]=tex;
	config.textures[config.textures_size]=tex;
	config.textures_size++;
	return tex;
}

static inline void drawLight(vec2 * pos, float size) __attribute__((always_inline));
static inline void drawLight(vec2 * pos, float size){
	glPushMatrix();
	glTranslatef(pos->x,pos->y,0);
	backTransform();
	glTranslatef(0,0.2,0);
		Begin(GL_TRIANGLE_FAN);
			TexCoord2f (0.0f, 0.0f);
			Vertex2f(-size,-size);
			TexCoord2f (0.0f, 1.0f);
			Vertex2f(-size,size);
			TexCoord2f (1.0f, 1.0f);
			Vertex2f(size,size);
			TexCoord2f (1.0f, 0.0f);
			Vertex2f(size,-size);
		End();
	glPopMatrix();
}

void drawLights(){
	int i;
	setTexture(&config.map.tex[LIGHT]);
	Color4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
//	glPushMatrix();
//	globalTransform();
		for(i=0;i<config.map.tower_max;i++)
			if (config.map.tower_array[i].id!=0)
				if (checkGridLines(config.map.tower_array[i].position.x,config.map.tower_array[i].position.y)){
					int owner=config.map.tower_array[i].owner;
					float size=1; //if less -> more transparent; black -> transparent, bright -> black;
					if (owner!=config.map.player_id)
						size=(config.map.players[owner].group==config.map.player->group?0.7f:0.0f);
					drawLight(&config.map.tower_array[i].position,1.5*size);
				}
		for(i=0;i<config.map.npc_max;i++)
			if (config.map.npc_array[i].id!=0)
				if (checkGridLines(config.map.npc_array[i].position.x,config.map.npc_array[i].position.y)){
					int owner=config.map.npc_array[i].owner;
					float size=1;
					if (owner!=config.map.player_id)
						size=(config.map.players[owner].group==config.map.player->group?0.7f:0.0f);
					drawLight(&config.map.npc_array[i].position,1*size);	
				}
//	glPopMatrix();
}
#define scale 4.0f
void drawLightsMap(){
//	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
//	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
//	glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
	glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
	
	
/*	if (config.options.darkness.tex_size>128){
		setTexture(&config.map.tex[DARKNESS]);
		Begin(GL_TRIANGLE_FAN);
			TexCoord2f (config.global.screen.coord.ld.x/scale,config.global.screen.coord.ld.y/scale);
			Vertex2f(config.global.screen.coord.ld.x,config.global.screen.coord.ld.y);
			TexCoord2f(config.global.screen.coord.lu.x/scale,config.global.screen.coord.lu.y/scale);
			Vertex2f(config.global.screen.coord.lu.x,config.global.screen.coord.lu.y);
			TexCoord2f(config.global.screen.coord.ru.x/scale,config.global.screen.coord.ru.y/scale);
			Vertex2f(config.global.screen.coord.ru.x,config.global.screen.coord.ru.y);
			TexCoord2f(config.global.screen.coord.rd.x/scale,config.global.screen.coord.rd.y/scale);
			Vertex2f(config.global.screen.coord.rd.x,config.global.screen.coord.rd.y);
		End();
	}
*/
	drawLights(drawLight);
	
//	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void getLightsMask(){
#define width config.options.window.width
#define height config.options.window.height
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glViewport(0, 0,  config.options.darkness.tex_size, config.options.darkness.tex_size);
	
	
	drawLightsMap();
	setTexture(lights);
//	glBindTexture(GL_TEXTURE_2D,config.map.tex[LIGHT_MASK].tex[0]);

//        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, config.options.lights.tex_width, config.options.lights.tex_height, 0);
//     glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, 0, 0, config.options.lights.tex_width, config.options.lights.tex_height);
	glReadPixels(0, 0, config.options.darkness.tex_size, config.options.darkness.tex_size, GL_RED, GL_UNSIGNED_BYTE, mem);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, config.options.darkness.tex_size, config.options.darkness.tex_size, 0, GL_ALPHA, GL_UNSIGNED_BYTE, mem);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, width, height);
#undef width
#undef height
}

void drawLightsMask(){
	Color4f(1,1,1,0.95);
	setTexture(lights);
//	glEnable(GL_TEXTURE_2D);
//	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
	Begin(GL_TRIANGLE_FAN);
		TexCoord2f (0.01f, 0.01f);
		Vertex2f(0,0);
		TexCoord2f (0.01f, 0.99f);
		Vertex2f(0,config.options.window.height);
		TexCoord2f (0.99f, 0.99f);
		Vertex2f(config.options.window.width,config.options.window.height);
		TexCoord2f (0.99f, 0.01f);
		Vertex2f(config.options.window.width,0);
	End();
//	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}
