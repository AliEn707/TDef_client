#include "headers.h"
/*
NOT WORKED YET
*/

void DrawCircle(float cx, float cy, float r, int num_segments) 
{ 
	float theta = 2 * 3.1415926 /num_segments; 
	float c = cosf(theta);//precalculate the sine and cosine
	float s = sinf(theta);
	float t;

	float x = r;//we start at angle = 0 
	float y = 0; 
	int ii;
	
	glBegin(GL_TRIANGLE_FAN); 
	for(ii = 0; ii < num_segments; ii++) 
	{ 
		glVertex2f(x + cx, y + cy);//output vertex 
        
		//apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	} 
	glEnd(); 
}

void drawLightMask(vec2 * pos, float size){
	setTexture(&config.map.tex[LIGHT_MASK]);
	glEnable(GL_TEXTURE_2D);
	DrawCircle(pos->x,pos->y,size*.85,15);
}

void drawLight(vec2 * pos, float size){
	setTexture(&config.map.tex[LIGHT_MASK]);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f (0.0f, 0.0f);
		glVertex2f(pos->x-size,pos->y-size);
		glTexCoord2f (0.0f, 1.0f);
		glVertex2f(pos->x-size,pos->y+size);
		glTexCoord2f (1.0f, 0.0f);
		glVertex2f(pos->x+size,pos->y-size);
		glTexCoord2f (1.0f, 1.0f);
		glVertex2f(pos->x+size,pos->y+size);
	glEnd();
}

void drawLights(void (*draw)(vec2 * pos, float size) ){
	int i;
	glPushMatrix();
	globalTransform();
		for(i=0;i<config.map.tower_max;i++)
			if (config.map.tower_array[i].id!=0)
				draw(&config.map.tower_array[i].position,1);
		for(i=0;i<config.map.npc_max;i++)
			if (config.map.npc_array[i].id!=0)
				draw(&config.map.npc_array[i].position,1);	
	glPopMatrix();
}

void getLightsMask(){
/*	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	glStencilFunc(GL_NEVER, 1, 0xFF);
	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);  // draw 1s on test fail (always)

	// draw stencil pattern
	glStencilMask(0xFF);
	glClear(GL_STENCIL_BUFFER_BIT);  // needs mask=0xFF
	drawLights(drawLightMask);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
	glDisable(GL_STENCIL_TEST);
*/
}

void drawLightsMask(){
	glColor4f(0,0,0,0.9);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2f(0,0);
		glVertex2f(0,config.window_height);
		glVertex2f(config.window_width,0);
		glVertex2f(config.window_width,config.window_height);
	glEnd();
	
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glColor4f(1,1,1,0.5);
	drawLights(drawLight);
	
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
/*
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0x00);
	// draw where stencil's value is 0
	glStencilFunc(GL_EQUAL, 0, 0xFF);
	
	glColor4f(0,0,0,0.9);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2f(0,0);
		glVertex2f(0,config.window_height);
		glVertex2f(config.window_width,0);
		glVertex2f(config.window_width,config.window_height);
	glEnd();
	
	// draw only where stencil's value is 1
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	//where objects are 
	drawLights(drawLight);
	glDisable(GL_STENCIL_TEST);
*/
}
