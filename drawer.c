#include "main.h"

void drawCursor(){
	glColor4f(cursor.color.r,cursor.color.g,cursor.color.b,1);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINE_LOOP);
	glVertex2f(cursor.state.x+20,cursor.state.y);
	glVertex2f(cursor.state.x,cursor.state.y);
	glVertex2f(cursor.state.x,cursor.state.y-20);
	
	glEnd();
	
	
}

void drawElement(element * e,int focus){
	if (e->tex!=0)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
	if (focus==0)
		glColor4f(e->color.r,e->color.g,e->color.b,1.0);
	else
		glColor4f(e->fcolor.r,e->fcolor.g,e->fcolor.b,1.0);
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
}

void drawObject(object * o){
	int i;
//	printf("%g %g|%g %g\n",o->position.x,o->position.y,o->size.x,o->size.y);
	glPushMatrix();
		if(1){ 
			glDisable(GL_TEXTURE_2D);
			glColor4f(1,1,1,1);
			glBegin(GL_LINE_LOOP);
				glVertex2f(o->position.x,o->position.y);
				glVertex2f(o->position.x,o->position.y+o->size.y);
				glVertex2f(o->position.x+o->size.x,o->position.y+o->size.y);
				glVertex2f(o->position.x+o->size.x,o->position.y);
			glEnd();
		}
		glTranslatef(o->position.x,o->position.y,0);
		for(i=0;i<o->elements_size;i++)
			drawElement(&o->elements[i],o->in_focus);
	glPopMatrix();
}

void drawMenu(){
	menu* m=0;
	m=&config.menu.root;
	int i;
	for(i=0;i<config.menu.depth;i++)
		m=&m->submenu[config.menu.path[i]];
	for(i=0;i<m->objects_size;i++)
		drawObject(&m->objects[i]);
	
}

void drawNode(gnode * n){
	glBindTexture(GL_TEXTURE_2D, n->tex);
	glEnable(GL_TEXTURE_2D);
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
	glDisable(GL_TEXTURE_2D);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
//	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
		globalTransform();
		glDisable(GL_DEPTH_TEST);
		//draw map
		glEnable(GL_DEPTH_TEST);
		//draw map egain
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
	//draw screen controls
	
	if (config.menu.enable!=0){
		drawMenu();
	}
	//must be the last
	drawCursor(); 
}

