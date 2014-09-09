#include "headers.h"

int textureFrameNext(texture *t){
	if (config.texture_no_change==0)
		t->current_frame+=Df;
	if (t->current_frame>=t->frames){
		if (t->loop==0){
			t->current_frame=t->frames-1;
			if (t->lf_delay_counter<=t->lf_delay){
				t->lf_delay_counter++;
				return 0;
			}
				
		}else
			t->current_frame=0;
		return 1;
	}		
	return 0;
}

void drawText(GLFONT *glFont,char* text){
	//glFontHeight(glFont,text)
	glFontTextOut(glFont,text,0,0,0);
}

void drawTextCentered(GLFONT *glFont,char* text){
	glPushMatrix();
	glTranslatef(-glFontWigth(glFont,text)/2,0,0);
	glFontTextOut(glFont,text,0,0,0);
	glPopMatrix();
}



void drawCursor(){
	glColor4f(cursor.color.r,cursor.color.g,cursor.color.b,1);
	glDisable(GL_DEPTH_TEST);
	setTexture(&cursor.tex);
	float cur_size=50;
	glBegin(GL_QUADS);
		glVertex2f(cursor.state.x+cur_size,cursor.state.y-cur_size);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(cursor.state.x,cursor.state.y-cur_size);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(cursor.state.x,cursor.state.y);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(cursor.state.x+cur_size,cursor.state.y);
		glTexCoord2f(1.0f, 0.0f);
	glEnd();
	if (cursor.text!=0)
		if (cursor.text[0]!=0){
			float length;
			float height=glFontHeight(&mainfont,cursor.text);
			float shift=0.05f;
			length=glFontWigth(&mainfont,cursor.text);
			glPushMatrix();
				glDisable(GL_TEXTURE_2D);
				//glTranslatef(cursor.state.x,cursor.state.y,0);
				glTranslatef(cursor.state.x+42,cursor.state.y-25,0);
				glScalef(15,15,1); //need to correct
				glTranslatef(-length,0,0);
				//glTranslatef(-length/2.0f,height,0);
				glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
				glBegin(GL_TRIANGLE_STRIP);
					glVertex3f(-shift,shift,0);
					glVertex3f(-shift,-height-shift,0);
					glVertex3f(length+shift,shift,0);
					glVertex3f(length+shift,-height-shift,0);
				glEnd();
					glEnable(GL_TEXTURE_2D);
					glColor4f(1.0f, 1.0f, 1.0f,1.0f);
					drawText(&mainfont,cursor.text);
			glPopMatrix();
		}
}

int setTexture(texture * t){
	if (t->frames==0){
		glDisable(GL_TEXTURE_2D);
		return 0;
	}
	if (t->fd_delay_counter<t->fd_delay){
		t->fd_delay_counter++;
		glColor4f(0,0,0,0);
		return 0;
	}
	//add some stuff
	glBindTexture(GL_TEXTURE_2D, t->tex[(int)t->current_frame]);
	glEnable(GL_TEXTURE_2D);
	//if (config.global_count%FpF==0) //check next texture
	return textureFrameNext(t);
}

void drawElement(element * e,int focus){
	if (focus==0){
		glColor4f(e->color.r,e->color.g,e->color.b,e->color.a);
		if (e->focus_tex!=0)
			setTexture(&e->ftex);
		else
			setTexture(&e->tex);
	}else{
		glColor4f(e->fcolor.r,e->fcolor.g,e->fcolor.b,e->fcolor.a);
		setTexture(&e->tex);
	}
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
	if (*e->text==0)
		return;
	glPushMatrix();
		glTranslatef(e->position.x+e->text_position.x,
				e->position.y+e->text_position.y,
				0);
		glScalef(15,15,1);
		glTranslatef(0,glFontHeight(&mainfont,e->text),0);
		if (e->text_centered!=0)
			drawTextCentered(&mainfont,e->text);
		else
			drawText(&mainfont,e->text);
	glPopMatrix();
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
	glTranslatef(config.global.transform.translate.x,config.global.transform.translate.y,-100);
	glScalef(config.global.transform.scale,config.global.transform.scale,1);
	glRotatef(-60,1,0,0);
	//glScalef(1,0.5,1);
	glRotatef(-45,0,0,1);
	
}

void backTransform(){
	glRotatef(45,0,0,1);
	glRotatef(60,1,0,0);
	//glScalef(1,4*1,1);
}

void drawHealth(vec2 pos,vec2 size,float p){
	glPushMatrix();
	glTranslatef(0,0,0.03);
	glDisable(GL_TEXTURE_2D);
	glColor4f(0,0,0,0.8);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2f(pos.x+size.x*p,pos.y);
		glVertex2f(pos.x+size.x,pos.y);
		glVertex2f(pos.x+size.x*p,pos.y+size.y);
		glVertex2f(pos.x+size.x,pos.y+size.y);
	glEnd();
	glColor4f(1.2*(1-p),0.8*p,0,1);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2f(pos.x,pos.y);
		glVertex2f(pos.x+size.x*p,pos.y);
		glVertex2f(pos.x,pos.y+size.y);
		glVertex2f(pos.x+size.x*p,pos.y+size.y);
		
	glEnd();
	glPopMatrix();
}

void drawNpc(npc* n){
	glPushMatrix();
	glTranslatef(n->position.x,n->position.y,0);
	backTransform();
//	printf("n->type - %d\n",n->type);
	glScalef(0.9,0.9,1);
	glTranslatef(0,0,1);
		if (n->tex[n->current_tex].frames==0){
			if (config.npc_types[n->type].tex[n->current_tex].frames==0)
				//npc stored in global tex memory
				loadTexture(&config.npc_types[n->type].tex[n->current_tex],config.npc_types[n->type].tex_path[n->current_tex]);
			memcpy(&n->tex[n->current_tex],&config.npc_types[n->type].tex[n->current_tex],sizeof(texture));
		}
		if(setTexture(&n->tex[n->current_tex]))
			n->anim_ended=1;
//		else
//			n->anim_ended=0;
		glColor4f(1,1,1,1);
//		glBegin(GL_LINE_LOOP);
//		glBegin(GL_QUADS);
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f (0.005f, 0.005f);
			glVertex2f(-0.5f,0.0f);
			glTexCoord2f (0.995f, 0.005f);
			glVertex2f(0.5f,0.0f);
			glTexCoord2f (0.005f, 0.995f);
			glVertex2f(-0.5f,1.0f);
			glTexCoord2f (0.995f, 0.995f);
			glVertex2f(0.5f,1.0f);
		glEnd();
		//draw health
		float health=1.0*n->health/config.npc_types[n->type].health;
		if (health<0.95)
			drawHealth((vec2){-0.5,0.9},(vec2){0.9,0.09},health);
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
		glScalef(1.2,1.2,1);
		glTranslatef(0,0.2,1);
//		printf("tower %d health %d on %d\n",t->id,t->health,posToId(t->position));
		if (t->tex[t->current_tex].frames==0){
			if (config.tower_types[t->type].tex[t->current_tex].frames==0)
				//npc stored in global tex memory
				loadTexture(&config.tower_types[t->type].tex[t->current_tex],config.tower_types[t->type].tex_path[t->current_tex]);
			memcpy(&t->tex[t->current_tex],&config.tower_types[t->type].tex[t->current_tex],sizeof(texture));
		}
		if(setTexture(&t->tex[t->current_tex]))
			t->anim_ended=1;
		
			glColor4f(1,1,1,1);
//			glBegin(GL_LINE_LOOP);
			glBegin(GL_QUADS);
				glTexCoord2f (0.005f, 0.005f);
				glVertex2f(-0.5f,-0.5f);
				glTexCoord2f (0.995f, 0.005f);
				glVertex2f(0.5f,-0.5f);
				glTexCoord2f (0.995f, 0.995f);
				glVertex2f(0.5f,0.5f);
				glTexCoord2f (0.005f, 0.995f);
				glVertex2f(-0.5f,0.5f);
			glEnd();
		
			float health=1.0*t->health/config.tower_types[t->type].health;
			if (health<0.95)
				drawHealth((vec2){-0.325,0.5},(vec2){0.75,0.075},health);
	glPopMatrix();
}

void drawTowers(){
	int i;
	for(i=0;i<config.map.tower_max;i++)
		if (config.map.tower_array[i].id!=0)
			drawTower(&config.map.tower_array[i]);
}

void drawBullet(bullet* b){
	glPushMatrix();
//	printf("%g %g| %g %g %d\n",b->position.x,b->position.y,b->direction.x,b->direction.y,b->type);
	vec2* pos;
	float length=1;
//	config.bullet_types[b->type].solid=1;
	if (config.bullet_types[b->type].solid!=0)
		pos=&b->source;
	else
		pos=&b->position;
	
	glTranslatef(pos->x,pos->y,0);
	backTransform();
	glTranslatef(0,0.2,1.01);
	float x;
	
	if (b->destination.x==pos->x && b->destination.y==pos->y)
		pos=&b->source;
//	else
	{
		float ang;
		vec2 dir={b->destination.x-pos->x,b->destination.y-pos->y};
		length=sqrt(sqr(dir.x)+sqr(dir.y));
		
		float y;
		x=gridToScreenX(b->destination.x,b->destination.y);
		y=gridToScreenY(b->destination.x,b->destination.y);
		x-=gridToScreenX(pos->x,pos->y);
		y-=gridToScreenY(pos->x,pos->y);
//		printf("%g %g| %g %g\n",dir.x,b->direction.x,dir.y,b->direction.y);
		if (sign(b->direction.x)!=sign(dir.x))
			goto drawBulletExit;
//			x*=-1;
		if (sign(b->direction.y)!=sign(dir.y))
			goto drawBulletExit;
//			y*=-1;
//		if (x==0 )
//			goto drawBulletExit;
		
		float l=sqrt(sqr(x)+sqr(y));
		float cos=x/l;
		float sin=y/l;
		ang=acosf(cos);
		ang=ang*180/M_PI;
		if (y<0)
			ang*=-1;
		
		length*=0.5+0.5*(1-fabs(sin));
		glRotatef(ang,0,0,1);
	}
	if (b->tex[b->current_tex].frames==0){
		if (config.bullet_types[b->type].tex[b->current_tex].frames==0)
			loadTexture(&config.bullet_types[b->type].tex[b->current_tex],config.bullet_types[b->type].tex_path[b->current_tex]);
		memcpy(&b->tex[b->current_tex],&config.bullet_types[b->type].tex[b->current_tex],sizeof(texture));
	}
	float tx1,ty1,tx2,ty2,vx;
	float height=0.08f;
	if (config.bullet_types[b->type].solid==0){
		tx1=0.005f;
		ty1=0.005f;
		tx2=0.995f;
		ty2=0.995f;
		vx=height*8;
	}else{
		tx1=0.005f;
		ty1=0.005f;
		tx2=length/0.64;
		ty2=0.995f;
		vx=length;
	}
	if (x<0){
		x=ty1;
		ty1=ty2;
		ty2=x;
	}
	
//	else
//		b->anim_ended=0;
		glColor4f(1,1,1,1);
		if (setTexture(&b->tex[b->current_tex]))
			b->anim_ended=1;
//		glBegin(GL_LINE_LOOP);
		glBegin(GL_QUADS);
			glTexCoord2f (tx1, ty1);
			glVertex2f(0.0f,-height);
			glTexCoord2f (tx1, ty2);
			glVertex2f(0.0f,height);
			glTexCoord2f (tx2, ty2);
			glVertex2f(vx,height);
			glTexCoord2f (tx2, ty1);
			glVertex2f(vx,-height);
			//texture 1:2
			//texture solid 1:4
			
		glEnd();
drawBulletExit:		
	glPopMatrix();
}

void drawBullets(){
	int i;
	for(i=0;i<config.map.bullet_max;i++)
		if (config.map.bullet_array[i].id!=0)
			drawBullet(&config.map.bullet_array[i]);
}


void drawSplash(splash* s){
	glPushMatrix();
	glTranslatef(s->position.x,s->position.y,0);
	backTransform();
//	glScalef(1.2,1.2,1);
	glTranslatef(0,0,1);
//	printf("tower %d health %d on %d\n",t->id,t->health,posToId(t->position));
	if (s->tex.frames==0){
		if (config.splash_types[s->type].tex.frames==0)
			loadTexture(&config.splash_types[s->type].tex,config.splash_types[s->type].tex_path);
		memcpy(&s->tex,&config.splash_types[s->type].tex,sizeof(texture));
	}
	if(setTexture(&s->tex))
		s->anim_ended=1;
	
		glColor4f(1,1,1,1);
//		glBegin(GL_LINE_LOOP);
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f (0.0f, 0.0f);
			glVertex2f(-0.5f,0.0f);
			glTexCoord2f (1.0f, 0.0f);
			glVertex2f(0.5f,0.0f);
			glTexCoord2f (0.0f, 1.0f);
			glVertex2f(-0.5f,1.0f);
			glTexCoord2f (1.0f, 1.0f);
			glVertex2f(0.5f,1.0f);
		glEnd();
	glPopMatrix();
}

void drawSplashes(){
	int i;
	for(i=0;i<config.map.tower_max;i++)
		if (config.map.tower_array[i].type!=0)
			drawTower(&config.map.tower_array[i]);
}

void drawWall(wall* w){
	glPushMatrix();
	glTranslatef(getGridX(w->position),getGridY(w->position),0);
//	glTranslatef(0,0.2,0);
//	printf("tower %d health %d on %d\n",t->id,t->health,posToId(t->position));
	float x=0;
	float y=0;
	vec2 shift={0,0};
	#define size 0.65f
	if (w->direction=='x'){
		x=size;
		shift.x=size-0.5f;
	}else {
		y=size;
		shift.y=-(size-0.5f);
	}
	glTranslatef(shift.x,shift.y,0);
	setTexture(&config.map.tex[w->tex]);
		glColor4f(1,1,1,1);
//		glBegin(GL_LINE_LOOP);
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f (0.005f, 0.005f);
			glVertex3f(-x,y,0);
			glTexCoord2f (0.995f, 0.005f);
			glVertex3f(x,-y,0);
			glTexCoord2f (0.005f, 0.995f);
			glVertex3f(-x,y,2*size);
			glTexCoord2f (0.995f, 0.995f);
			glVertex3f(x,-y,2*size);
		glEnd();
	glPopMatrix();
	#undef size
}

void drawWalls(){
	int i;
	for(i=0;i<config.map.walls_size;i++)
		if (config.map.walls[i].direction!=0)
			drawWall(&config.map.walls[i]);
}

void drawMapObject(map_object* o){
	glPushMatrix();
	glTranslatef(o->position.x,o->position.y,0);
	backTransform();
	glScalef(1.415,1.415,1);
//quad size 1.415 
//if first 2,3 and second 3,4.1 -> |   |   |	
//if first 2,3 and second 3,4 -> |      |	
//	glTranslatef(0,0.2,0);
//	printf("object on map\n");
	setTexture(&config.map.tex[o->tex]);
		glColor4f(1,1,1,1);
//		glBegin(GL_LINE_LOOP);
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f (0.0f, 0.0f);
			glVertex2f(-0.5f,0.0f);
			glTexCoord2f (1.0f, 0.0f);
			glVertex2f(0.5f,0.0f);
			glTexCoord2f (0.0f, 1.0f);
			glVertex2f(-0.5f,1.0f);
			glTexCoord2f (1.0f, 1.0f);
			glVertex2f(0.5f,1.0f);
		glEnd();
	glPopMatrix();
}

void drawMapObjects(){
	int i;
	for(i=0;i<config.map.map_objects_size;i++)
		if (config.map.map_objects[i].tex!=0)
			drawMapObject(&config.map.map_objects[i]);
}

void drawScene(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
//	glEnable(GL_DEPTH_TEST);
	//when we load something
	if (config.loading.enable!=0){
		drawMenu(&config.loading);
		goto out;
	}
	if (config.auth==0){
		drawMenu(&config.auth_menu);
		goto out;
	}
	if (config.map.enable!=0){
		glPushMatrix();
			globalTransform();
			glDisable(GL_DEPTH_TEST);
			drawMap();
		
			config.texture_no_change=1;
			drawWalls();
	//		glEnable(GL_DEPTH_TEST);
	//		config.texture_no_change=0;
	//		drawWalls();
			
			glDisable(GL_DEPTH_TEST);
			config.texture_no_change=1;
			drawMapObjects();
			drawNpcs();
			drawTowers();
			drawBullets();
			drawSplashes();
			//draw map
			glEnable(GL_DEPTH_TEST);
			config.texture_no_change=0;
			drawWalls();
			drawMapObjects();
			drawNpcs();
			drawTowers();
			drawBullets();
			drawSplashes();
			//draw map egain
		glPopMatrix();
		glDisable(GL_DEPTH_TEST);
	}
	//draw screen controls
	
	if (config.menu.enable!=0){
		drawMenu(&config.menu.root);
	}else{
		drawMenu(&config.map.screen_menu);
		if (config.map.action_menu.enable!=0)
			drawMenu(&config.map.action_menu);
	}
out:
	//must be the last
	drawCursor(); 
	
	glFlush();
	SDL_GL_SwapWindow(config.window);
}

