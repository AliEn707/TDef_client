#include "main.h"
#include "drawer.h"

void drawCursor(){
	glColor4f(cursor.color.r,cursor.color.g,cursor.color.b,1);
	glDisable(GL_DEPTH_TEST);
	setTexture(&cursor.tex);
	glBegin(GL_QUADS);
	glVertex2f(cursor.state.x+40,cursor.state.y-40);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(cursor.state.x,cursor.state.y-40);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(cursor.state.x,cursor.state.y);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(cursor.state.x+40,cursor.state.y);
	glTexCoord2f(1.0f, 0.0f);
	glEnd();
}

void setTexture(texture * t){
	if (t->frames==0){
		glDisable(GL_TEXTURE_2D);
		return;
	}
	//add some stuff
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, t->tex[t->current_frame]);
	if (config.global_count%FpF==0) //check next texture
		textureFrameNext(t);
}

void drawElement(element * e,int focus){
	setTexture(&e->tex);
	if (focus==0)
		glColor4f(e->color.r,e->color.g,e->color.b,e->color.a);
	else
		glColor4f(e->fcolor.r,e->fcolor.g,e->fcolor.b,e->fcolor.a);
	e->wire==0?glBegin(GL_QUADS):glBegin(GL_LINE_LOOP);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(e->position.x,e->position.y);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(e->position.x,e->position.y+e->size.y);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(e->position.x+e->size.x,e->position.y+e->size.y);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(e->position.x+e->size.x,e->position.y);
	glEnd();
}

void drawObject(object * o){
	int i;
//	printf("%g %g|%g %g\n",o->position.x,o->position.y,o->size.x,o->size.y);
	glPushMatrix();
		glTranslatef(o->position.x,o->position.y,0);
		for(i=0;i<o->elements_size;i++)
			drawElement(&o->elements[i],o->in_focus);
	glPopMatrix();
}

void drawMenu(menu * root){
	menu* m=0;
	m=root;
	int i;
	glDisable(GL_TEXTURE_2D);
	for(i=0;i<config.menu.depth;i++)
		m=&m->submenu[config.menu.path[i]];
	for(i=0;i<m->objects_size;i++)
		drawObject(&m->objects[i]);
	
}

void drawNode(gnode * n){
//	glBindTexture(GL_TEXTURE_2D, n->tex);
//	glEnable(GL_TEXTURE_2D);
	setTexture(&config.map.tex[n->tex]);
	if (n->id==config.map.focus && config.menu.selected==0)
		glColor4f(0,1,0,1);
	else
		glColor4f(1,1,1,1);
	glPushMatrix();
		if (n->id>=0)
			glTranslatef(n->id/config.map.grid_size,n->id%config.map.grid_size,0);
//		glBegin(GL_LINE_LOOP);
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
		if (n->buildable!=0){
			setTexture(&config.map.tex[BUILDABLE]);
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
		if (config.map.show_walk!=0){
			glColor4f(1,1,1,1);
			if (n->walkable<1){
				if (n->walkable==0)
					setTexture(&config.map.tex[WALKABLE]);
				else
					setTexture(&config.map.tex[NO_SEE]);
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
		}
	glPopMatrix();	
	glDisable(GL_TEXTURE_2D);
}


void drawMap(){
	int i,j;
	for(i=0;i<config.map.grid_size;i++)
		for(j=0;j<config.map.grid_size;j++)
			drawNode(&config.map.grid[to2d(i,j)]);
	//draw non working zone
	int k;
	k=0;
	for(i=-1;i>-(config.map.grid_size/2+config.map.grid_size%2+1);i--)
		for(j=-i-1;j<config.map.grid_size-(-i-1);j++){
			glPushMatrix();
				glTranslatef(i,j,0);
				drawNode(&config.map.grid_out[0][k]);
			glPopMatrix();
			k++;
		}
	k=0;
	for(j=0;j<(config.map.grid_size/2+config.map.grid_size%2+1);j++)
		for(i=j;i<config.map.grid_size-j;i++){
			glPushMatrix();
				glTranslatef(i,config.map.grid_size+j,0);
				drawNode(&config.map.grid_out[1][k]);
			glPopMatrix();
			k++;
		}
	k=0;
	for(i=0;i<(config.map.grid_size/2+config.map.grid_size%2+1);i++)
		for(j=i;j<config.map.grid_size-i;j++){
			glPushMatrix();
				glTranslatef(config.map.grid_size+i,j,0);
				drawNode(&config.map.grid_out[2][k]);
			glPopMatrix();
			k++;
		}
	k=0;
	for(j=-1;j>-(config.map.grid_size/2+config.map.grid_size%2+1);j--)
		for(i=-j-1;i<config.map.grid_size-(-j-1);i++){
			glPushMatrix();
				glTranslatef(i,j,0);
				drawNode(&config.map.grid_out[3][k]);
			glPopMatrix();
			k++;
		}
}

void globalTransform(){
	
	glTranslatef(config.map.transform.translate.x,config.map.transform.translate.y,-100);
	glRotatef(-60,1,0,0);
	glScalef(config.map.transform.scale,config.map.transform.scale,1);
	//glScalef(1,0.5,1);
	glRotatef(-45,0,0,1);
}

void backTransform(){
	glRotatef(45,0,0,1);
	glRotatef(60,1,0,0);
	glScalef(0.8,4*0.8,1);
}


void drawNpc(npc* n){
	glPushMatrix();
	glTranslatef(n->position.x,n->position.y,0);
	backTransform();
		glColor4f(rand()%100/100.0,rand()%100/100.0,rand()%100/100.0,1);
//		glBegin(GL_LINE_LOOP);
		glBegin(GL_QUADS);
			glTexCoord2f (0.0f, 0.0f);
			glVertex2f(-0.5f,0.0f);
			glTexCoord2f (1.0f, 0.0f);
			glVertex2f(0.5f,0.0f);
			glTexCoord2f (1.0f, 1.0f);
			glVertex2f(0.5f,1.0f);
			glTexCoord2f (0.0f, 1.0f);
			glVertex2f(-0.5f,1.0f);
		glEnd();
	
	glPopMatrix();
}

void drawNpcs(){
	int i;
	for(i=0;i<config.map.npc_max;i++)
		if (config.map.npc_array[i].id!=0)
			drawNpc(&config.map.npc_array[i]);
}

void drawTower(tower* t){
	glPushMatrix();
	glTranslatef(t->position.x,t->position.y,0);
	backTransform();
		glColor4f(rand()%100/100.0,rand()%100/100.0,rand()%100/100.0,1);
//		glBegin(GL_LINE_LOOP);
		glBegin(GL_QUADS);
			glTexCoord2f (0.0f, 0.0f);
			glVertex2f(-0.5f,-0.5f);
			glTexCoord2f (1.0f, 0.0f);
			glVertex2f(0.5f,-0.5f);
			glTexCoord2f (1.0f, 1.0f);
			glVertex2f(0.5f,0.5f);
			glTexCoord2f (0.0f, 1.0f);
			glVertex2f(-0.5f,0.5f);
		glEnd();
	
	glPopMatrix();
}

void drawTowers(){
	int i;
	for(i=0;i<config.map.tower_max;i++)
		if (config.map.tower_array[i].id!=0)
			drawTower(&config.map.tower_array[i]);
}


void drawScene(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
//	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
		globalTransform();
		glDisable(GL_DEPTH_TEST);
		drawMap();
		//draw map
		glEnable(GL_DEPTH_TEST);
		drawNpcs();
		drawTowers();
		//draw map egain
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
	//draw screen controls
	
	
	if (config.menu.enable!=0){
		drawMenu(&config.menu.root);
	}else{
		drawMenu(&config.map.screen_menu);
		drawMenu(&config.map.action_menu);
	}
	//must be the last
	drawCursor(); 
}

