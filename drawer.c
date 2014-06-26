#include "main.h"


void drawElement(element * e){
	if (e->tex!=0)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
	glColor4f(e->color.r,e->color.g,e->color.b,1.0);
	e->wire==0?glBegin(GL_QUADS):glBegin(GL_LINE_LOOP);
		if (e->tex!=0)
			glTexCoord2f(0.0f, 0.0f);
		glVertex2f(e->position.x,e->position.y);
		if (e->tex!=0)
			glTexCoord2f(0.0f, 1.0f);
		glVertex2f(e->position.x,e->position.y+e->size.y);
		if (e->tex!=0)
			glTexCoord2f(1.0f, 1.0f);
		glVertex2f(e->position.x+e->size.x,e->position.y+e->size.y);
		if (e->tex!=0)
			glTexCoord2f(1.0f, 0.0f);
		glVertex2f(e->position.x+e->size.x,e->position.y);
	glEnd();
	if (e->tex!=0)
		glDisable(GL_TEXTURE_2D);
}

void drawObject(object * o){
	int i;
	glPushMatrix();
		glTranslatef(o->position.x,o->position.y,0);
		for(i=0;i<o->elements_size;i++)
			drawElement(&o->elements[i]);
	glPopMatrix();
}

void drawNode(gnode * n){
	glBindTexture(GL_TEXTURE_2D, n->tex);
	glPushMatrix();
		glTranslatef(n->id/config.map.gridsize,n->id%config.map.gridsize,0);
		glBegin(GL_QUADS);
			glTexCoord2f (0.0f, 0.0f);
			glVertex2f(0.0f,0.0f);
			glTexCoord2f (1.0f, 0.0f);
			glVertex2f(1.0f,0.0f);
			glTexCoord2f (1.0f, 1.0f);
			glVertex2f(1.0f,1.0f);
			glTexCoord2f (0.0f, 1.0f);
			glVertex2f(0.0f,1.0f);
		glEnd();
		if (n->walkable!=0){
			glBindTexture(GL_TEXTURE_2D, config.global_tex[WALKABLE]);
			glBegin(GL_QUADS);
				glTexCoord2f (0.0f, 0.0f);
				glVertex2f(0.0f,0.0f);
				glTexCoord2f (1.0f, 0.0f);
				glVertex2f(1.0f,0.0f);
				glTexCoord2f (1.0f, 1.0f);
				glVertex2f(1.0f,1.0f);
				glTexCoord2f (0.0f, 1.0f);
				glVertex2f(0.0f,1.0f);
			glEnd();
		}
		if (n->buildable!=0){
			glBindTexture(GL_TEXTURE_2D, config.global_tex[BUILDABLE]);
			glBegin(GL_QUADS);
				glTexCoord2f (0.0f, 0.0f);
				glVertex2f(0.0f,0.0f);
				glTexCoord2f (1.0f, 0.0f);
				glVertex2f(1.0f,0.0f);
				glTexCoord2f (1.0f, 1.0f);
				glVertex2f(1.0f,1.0f);
				glTexCoord2f (0.0f, 1.0f);
				glVertex2f(0.0f,1.0f);
			glEnd();
		}
	glPopMatrix();	
}

void globalTransform(){
/*	
	glTranslatef((tx),(ty),0);
	glRotatef(60,1,0,0);
	glScalef(sx,sy,1);
	//glScalef(1,0.5,1);
	glRotatef(-45,0,0,1);
*/
}

void drawScene(){
	glPushMatrix();
	//draw map
		globalTransform();
		
	glPopMatrix();
	//draw screen controls
	element e;
	e.position.x=0;
	e.position.y=0;
	e.size.x=100;
	e.size.y=100;
	e.color.r=1;
	e.color.g=1;
	e.color.b=0;
	e.wire=0;
	e.tex=0;
	
	glPushMatrix();
		glTranslatef(config.window_width/2,config.window_height/2,0);
			drawElement(&e);
	glPopMatrix();
}

