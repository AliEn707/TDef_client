#include "headers.h"

/*
╔══════════════════════════════════════════════════════════════╗
║functions for draw differrent elements				                       ║
║ 										                       ║
║ created by Dennis Yarikov						                       ║
║ jun 2014									                       ║
╚══════════════════════════════════════════════════════════════╝
*/

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

void drawText(char2* text){
//	glPushMatrix();		
//	glScalef(15,15,1);
//	glTranslatef(0,glFontHeight(&mainfont,text),0);
	glEnable(GL_TEXTURE_2D);
	glFontTextOut(text,0,0,0);
//	glPopMatrix();
}

void drawTextCentered(char2* text){
	glPushMatrix();		
//	glScalef(15,15,1);
	glTranslatef(-glFontWigth(text)/2,0,0);
	glEnable(GL_TEXTURE_2D);
	glFontTextOut(text,0,0,0);
	glPopMatrix();
}



void drawCursor(){
	Color4f(cursor.color.r,cursor.color.g,cursor.color.b,1);
	glDisable(GL_DEPTH_TEST);
	setTexture(&cursor.tex);
	float cur_size=50;
	glBegin(GL_TRIANGLE_FAN);
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
			char2 * cur_text=localeTextGet(cursor.text);
			float height=glFontHeight(cur_text);
			float shift=0.05f;
			length=glFontWigth(cur_text);
			glPushMatrix();
				glDisable(GL_TEXTURE_2D);
				//glTranslatef(cursor.state.x,cursor.state.y,0);
				glTranslatef(cursor.state.x+41,cursor.state.y-24,0);
				//glScalef(15,15,1); //need to correct
				glTranslatef(-length,0,0);
				//glTranslatef(-length/2.0f,height,0);
				Color4f(0.0f, 0.0f, 0.0f, 0.8f);
				glBegin(GL_TRIANGLE_FAN);
					glVertex3f(-shift,shift,0);
					glVertex3f(-shift,-height-shift,0);
					glVertex3f(length+shift+1,-height-shift,0);
					glVertex3f(length+shift+1,shift,0);
				glEnd();
					glEnable(GL_TEXTURE_2D);
					Color4f(1.0f, 1.0f, 1.0f,1.0f);
					drawText(cur_text);
			glPopMatrix();
		}
}

int setTexture(texture * t){
	if (t->frames==-1){
		glDisable(GL_TEXTURE_2D);
		return 0;
	}
	if (t->fd_delay_counter<t->fd_delay){
		t->fd_delay_counter++;
		Color4f(0,0,0,0);
		return 0;
	}
	//add some stuff
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, t->tex[(int)t->current_frame]);
	//if (config.global_count%FpF==0) //check next texture
	return textureFrameNext(t);
}

void drawElement(element * e,int focus){
	if (e->focus_tex!=0)
		if (e->ftex.frames==0){
			if(e->map!=0)
				loadMTexture(&e->ftex,e->ftex_path);
			else
				loadTexture(&e->ftex,e->ftex_path);
		}
	if (e->tex.frames==0){
		if(e->map!=0)
			loadMTexture(&e->tex,e->tex_path);
		else
			loadTexture(&e->tex,e->tex_path);
	}
	if (focus==0){
		Color4f(e->color.r,e->color.g,e->color.b,e->color.a);
		if (e->focus_tex!=0)
			setTexture(&e->ftex);
		else
			setTexture(&e->tex);
		if (e->color.a==0)
			goto texbreak;
	}else{
		Color4f(e->fcolor.r,e->fcolor.g,e->fcolor.b,e->fcolor.a);
		setTexture(&e->tex);
		if (e->fcolor.a==0)
			goto texbreak;
	}
	//to create slide animation
	if (focus==0){
		e->_position.x=e->position.x;
		e->_position.y=e->position.y;
	}else{
		e->_position.x=e->fposition.x;
		e->_position.y=e->fposition.y;
	}
	glPushMatrix();
	glTranslatef(e->_position.x,e->_position.y,0);
	vec2 tiles={1.0f,1.0f};
	if (e->tile_size.x!=0)
		tiles.x=e->size.x/e->tile_size.x;
	if (e->tile_size.y!=0)
		tiles.y=e->size.y/e->tile_size.y;
	
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0,0);
		glTexCoord2f(0.0f, tiles.y);
		glVertex2f(0,e->size.y);
		glTexCoord2f(tiles.x, tiles.y);
		glVertex2f(e->size.x,e->size.y);
		glTexCoord2f(tiles.x, 0.0f);
		glVertex2f(e->size.x,0);
	glEnd();
	
texbreak:
	//draw text
//	Color4f(1,1,1,1);
	//now draw wire
	if (focus==0){
		if (e->wirecolor.a==0)
			goto wirebreak;
		Color4f(e->wirecolor.r,e->wirecolor.g,e->wirecolor.b,e->wirecolor.a);
	}else{
		if (e->fwirecolor.a==0)
			goto wirebreak;
		Color4f(e->fwirecolor.r,e->fwirecolor.g,e->fwirecolor.b,e->fwirecolor.a);
	}
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINE_LOOP);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0,0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(0,e->size.y);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(e->size.x,e->size.y);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(e->size.x,0);
	glEnd();
	
wirebreak:	
	glEnable(GL_TEXTURE_2D);
	if (*e->text==0)
		goto exit;
	char2 * text=localeTextGet(e->text);
	glTranslatef(e->text_position.x,
				e->text_position.y,
				0);
	
		if (e->text_size!=0)
			glScalef(e->text_size,e->text_size,1);
		glTranslatef(0,glFontHeight(text),0);
		if (e->text_centered!=0)
			drawTextCentered(text);
		else
			drawText(text);
exit:
	glPopMatrix();
}

void drawObject(object * o){
	int i;
//	printf("%g %g|%g %g\n",o->position.x,o->position.y,o->size.x,o->size.y);
	if (config.menu.selected!=o)
		o->in_focus=0;
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
	float _x=0;
	float _y=0;
	if (config.map.tex[n->tex].frames==0){
		loadMTexture(&config.map.tex[n->tex],config.map.tex_path[n->tex]);
	}
	setTexture(&config.map.tex[n->tex]);
	if (n->id==config.map.focus && config.menu.selected==0)
		Color4f(0,1,0,1);
	else
		Color4f(1,1,1,1);
//	glPushMatrix();
	if (n->id>=0){
		_x=n->id/config.map.grid_size;
		_y=n->id%config.map.grid_size;
	}
//		if (n->id>=0)
//			glTranslatef(n->id/config.map.grid_size,n->id%config.map.grid_size,0);
//		glBegin(GL_LINE_LOOP);
		glBegin(GL_TRIANGLE_FAN);
			glTexCoord2f (0.0f, 0.0f);
			glVertex2f(_x,_y);
			glTexCoord2f (1.0f, 0.0f);
			glVertex2f(_x+1.0f,_y);
			glTexCoord2f (1.0f, 1.0f);
			glVertex2f(_x+1.0f,_y+1.0f);
			glTexCoord2f (0.0f, 1.0f);
			glVertex2f(_x,_y+1.0f);
		glEnd();
		if (n->buildable!=0){
			setTexture(&config.map.tex[BUILDABLE]);
			glBegin(GL_TRIANGLE_FAN);
				glTexCoord2f (0.0f, 0.0f);
				glVertex2f(_x,_y);
				glTexCoord2f (1.0f, 0.0f);
				glVertex2f(_x+1.0f,_y);
				glTexCoord2f (1.0f, 1.0f);
				glVertex2f(_x+1.0f,_y+1.0f);
				glTexCoord2f (0.0f, 1.0f);
				glVertex2f(_x,_y+1.0f);
			glEnd();
		}
		if (config.map.show_walk!=0){
			Color4f(1,1,1,1);
			if (n->walkable<1){
				if (n->walkable==0)
					setTexture(&config.map.tex[WALKABLE]);
				else
					setTexture(&config.map.tex[NO_SEE]);
				glBegin(GL_TRIANGLE_FAN);
					glTexCoord2f (0.0f, 0.0f);
					glVertex2f(_x,_y);
					glTexCoord2f (1.0f, 0.0f);
					glVertex2f(_x+1.0f,_y);
					glTexCoord2f (1.0f, 1.0f);
					glVertex2f(_x+1.0f,_y+1.0f);
					glTexCoord2f (0.0f, 1.0f);
					glVertex2f(_x,_y+1.0f);
				glEnd();
			}
		}
//	glPopMatrix();	
//	glDisable(GL_TEXTURE_2D);
}


void drawMap(){
	int i,j;
	for(i=0;i<config.map.grid_size;i++)
		for(j=0;j<config.map.grid_size;j++)
			if (checkGridLines(i,j))
				drawNode(&config.map.grid[to2d(i,j)]);
	//draw non working zone
	int k;
	k=0;
	for(i=-1;i>-(config.map.grid_size/2+config.map.grid_size%2+1);i--)
		for(j=-i-1;j<config.map.grid_size-(-i-1);j++)
			if (checkGridLines(i,j)){
				glPushMatrix();
					glTranslatef(i,j,0);
					drawNode(&config.map.grid_out[0][k]);
				glPopMatrix();
				k++;
			}
	k=0;
	for(j=0;j<(config.map.grid_size/2+config.map.grid_size%2+1);j++)
		for(i=j;i<config.map.grid_size-j;i++)
			if (checkGridLines(i,config.map.grid_size+j)){
				glPushMatrix();
					glTranslatef(i,config.map.grid_size+j,0);
					drawNode(&config.map.grid_out[1][k]);
				glPopMatrix();
				k++;
			}
	k=0;
	for(i=0;i<(config.map.grid_size/2+config.map.grid_size%2+1);i++)
		for(j=i;j<config.map.grid_size-i;j++)
			if (checkGridLines(config.map.grid_size+i,j)){
				glPushMatrix();
					glTranslatef(config.map.grid_size+i,j,0);
					drawNode(&config.map.grid_out[2][k]);
				glPopMatrix();
				k++;
			}
	k=0;
	for(j=-1;j>-(config.map.grid_size/2+config.map.grid_size%2+1);j--)
		for(i=-j-1;i<config.map.grid_size-(-j-1);i++)
			if (checkGridLines(i,j)){
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
//	glScalef(1,2,1);
	//glScalef(1,4*1,1);
}

void drawHealth(vec2 pos,vec2 size,float p){
//	glPushMatrix();
//	glTranslatef(0,0,0.03);//why?
	glDisable(GL_TEXTURE_2D);
	Color4f(0,0,0,0.8);
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(pos.x+size.x*p,pos.y);
		glVertex2f(pos.x+size.x,pos.y);
		glVertex2f(pos.x+size.x,pos.y+size.y);
		glVertex2f(pos.x+size.x*p,pos.y+size.y);
	glEnd();
	Color4f(1.2*(1-p),0.8*p,0,1);
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(pos.x,pos.y);
		glVertex2f(pos.x+size.x*p,pos.y);
		glVertex2f(pos.x+size.x*p,pos.y+size.y);
		glVertex2f(pos.x,pos.y+size.y);
		
	glEnd();
//	glPopMatrix();
}

void drawNpc(npc* n){
	float size=0.89f;
	if (config.npc_types[n->type].t_size!=0)
		size*=config.npc_types[n->type].t_size;
	glPushMatrix();
	glTranslatef(n->position.x,n->position.y,0);
	backTransform();
//	printf("n->type - %d\n",n->type);
	glScalef(size,size,1);
//	glTranslatef(0,0,.56f);
	glTranslatef(0,0,.06f);
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
		Color4f(1,1,1,1);
//		glBegin(GL_LINE_LOOP);
		glBegin(GL_TRIANGLE_FAN);
			glTexCoord2f (0.005f, 0.005f);
			glVertex2f(-0.5f,0.0f);
			glTexCoord2f (0.995f, 0.005f);
			glVertex2f(0.5f,0.0f);
			glTexCoord2f (0.995f, 0.995f);
			glVertex2f(0.5f,1.0f);
			glTexCoord2f (0.005f, 0.995f);
			glVertex2f(-0.5f,1.0f);
		glEnd();
		
//		glTranslatef(0,0,0.1169);
		glTranslatef(0,0,5);
		//draw health
		float health=1.0*n->health/config.npc_types[n->type].health;
		if (health<0.98)
			drawHealth((vec2){-0.5,0.9},(vec2){0.9,0.09},health);
//		glPushMatrix();
			Color4f(1,1,1,1);
			glTranslatef(-0.5,0.9,0);
			//glScalef(0.15,0.15,1);
			glScalef(0.011,0.011,1);
			char buf[5];
			sprintf(buf,"%hd",n->level);
			drawText(localeTextGet(buf));
//		glPopMatrix();
		
	glPopMatrix();
}

void drawNpcs(){
	int i;
	for(i=0;i<config.map.npc_max;i++)
		if (config.map.npc_array[i].id!=0)
			if (checkGridLines(config.map.npc_array[i].position.x,config.map.npc_array[i].position.y))
				drawNpc(&config.map.npc_array[i]);
}

void drawTower(tower* t){
	float size=1.4f;
	if (config.tower_types[t->type].t_size!=0)
		size*=config.tower_types[t->type].t_size;
	glPushMatrix();
		glTranslatef(t->position.x,t->position.y,0);
		backTransform();
		glScalef(size,size,1);
//		glTranslatef(0,0.21,.56f);
		glTranslatef(0,0.21,0.05f);
//		printf("tower %d health %d on %d\n",t->id,t->health,posToId(t->position));
		if (t->tex[t->current_tex].frames==0){
			if (config.tower_types[t->type].tex[t->current_tex].frames==0)
				//npc stored in global tex memory
				loadTexture(&config.tower_types[t->type].tex[t->current_tex],config.tower_types[t->type].tex_path[t->current_tex]);
			memcpy(&t->tex[t->current_tex],&config.tower_types[t->type].tex[t->current_tex],sizeof(texture));
		}
		if(setTexture(&t->tex[t->current_tex]))
			t->anim_ended=1;
		
			Color4f(1,1,1,1);
//			glBegin(GL_LINE_LOOP);
			glBegin(GL_TRIANGLE_FAN);
				glTexCoord2f (0.005f, 0.005f);
				glVertex2f(-0.5f,-0.5f);
				glTexCoord2f (0.995f, 0.005f);
				glVertex2f(0.5f,-0.5f);
				glTexCoord2f (0.995f, 0.995f);
				glVertex2f(0.5f,0.5f);
				glTexCoord2f (0.005f, 0.995f);
				glVertex2f(-0.5f,0.5f);
			glEnd();
			//draw level and health
			glTranslatef(0,0,5);//need to draw over all
			float health;
			if (t->type==BASE)
				health=1.0*t->health/config.map.players[t->owner].base_health;
			else
				health=1.0*t->health/config.tower_types[t->type].health;
			if (health<0.95)
				drawHealth((vec2){-0.325,0.5},(vec2){0.75,0.075},health);
			Color4f(1,1,1,1);
//		glPushMatrix();
			glTranslatef(-0.325,0.5,0);
			//glScalef(0.1125,0.1125,1);
			glScalef(0.0075,0.0075,1);
			char buf[5];
			sprintf(buf,"%d",t->level);
			drawText(localeTextGet(buf));
//		glPopMatrix();
	glPopMatrix();
}

void drawTowers(){
	int i;
	for(i=0;i<config.map.tower_max;i++)
		if (config.map.tower_array[i].id!=0)
			if (checkGridLines(config.map.tower_array[i].position.x,config.map.tower_array[i].position.y))
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
	
	if (b->destination.x==pos->x && b->destination.y==pos->y)
		pos=&b->source;
	
	glTranslatef(pos->x,pos->y,0);
	backTransform();
//	glTranslatef(0,0.2,1.01);
	glTranslatef(0,0.2,0.01);
	float x;
	
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
		if (config.bullet_types[b->type].solid==0){
			if (sign(b->direction.x)!=sign(dir.x))
				goto drawBulletExit;
	//			x*=-1;
			if (sign(b->direction.y)!=sign(dir.y))
				goto drawBulletExit;
		}
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
		Color4f(1,1,1,1);
		if (setTexture(&b->tex[b->current_tex]))
			b->anim_ended=1;
//		glBegin(GL_LINE_LOOP);
		glBegin(GL_TRIANGLE_FAN);
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
			if (config.bullet_types[config.map.bullet_array[i].type].solid!=0 || 
					checkGridLines(config.map.bullet_array[i].position.x,config.map.bullet_array[i].position.y))
				drawBullet(&config.map.bullet_array[i]);
}


void drawSplash(splash* s){
	glPushMatrix();
	glTranslatef(s->position.x,s->position.y,0);
	backTransform();
//	glScalef(1.2,1.2,1);
//	glTranslatef(0,0,1);
//	printf("tower %d health %d on %d\n",t->id,t->health,posToId(t->position));
	if (s->tex.frames==0){
		if (config.splash_types[s->type].tex.frames==0)
			loadTexture(&config.splash_types[s->type].tex,config.splash_types[s->type].tex_path);
		memcpy(&s->tex,&config.splash_types[s->type].tex,sizeof(texture));
	}
	if(setTexture(&s->tex))
		s->anim_ended=1;
	
		Color4f(1,1,1,1);
//		glBegin(GL_LINE_LOOP);
		glBegin(GL_TRIANGLE_FAN);
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

void drawSplashes(){
	int i;
	for(i=0;i<config.map.splash_max;i++)
		if (config.map.splash_array[i].type!=0)
			if (checkGridLines(config.map.splash_array[i].position.x,config.map.splash_array[i].position.y))
				drawSplash(&config.map.splash_array[i]);
}

void drawWall(wall* w){
	glPushMatrix();
	glTranslatef(getGridX(w->position),getGridY(w->position),0);
//	glTranslatef(0,0.2,0);
//	printf("tower %d health %d on %d\n",t->id,t->health,posToId(t->position));
	float x=0;
	float y=0;
	vec2 shift={0,0};
	#define size 0.645f
	if (w->direction=='x'){
		x=size;
		shift.x=size-0.5f;
	}else {
		y=size;
		shift.y=-(size-0.5f);
	}
	glTranslatef(shift.x,shift.y,0);
	if (config.map.tex[w->tex].frames==0){
		loadMTexture(&config.map.tex[w->tex],config.map.tex_path[w->tex]);
	}
	setTexture(&config.map.tex[w->tex]);
		Color4f(1,1,1,1);
//		glBegin(GL_LINE_LOOP);
		glBegin(GL_TRIANGLE_FAN);
			glTexCoord2f (0.005f, 0.005f);
			glVertex3f(-x,y,0);
			glTexCoord2f (0.995f, 0.005f);
			glVertex3f(x,-y,0);
			glTexCoord2f (0.995f, 0.995f);
			glVertex3f(x,-y,2*size);
			glTexCoord2f (0.005f, 0.995f);
			glVertex3f(-x,y,2*size);
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
	if (config.map.tex[o->tex].frames==0){
		loadMTexture(&config.map.tex[o->tex],config.map.tex_path[o->tex]);
	}
	setTexture(&config.map.tex[o->tex]);
		Color4f(1,1,1,1);
//		glBegin(GL_LINE_LOOP);
		glBegin(GL_TRIANGLE_FAN);
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

void drawMapObjects(){
	int i;
	for(i=0;i<config.map.map_objects_size;i++)
		if (config.map.map_objects[i].tex!=0)
			drawMapObject(&config.map.map_objects[i]);
}

void setMinimapObjectColor(int owner){
	if (owner==config.map.player_id)
		Color4f(0,1,0,1.0);
	else
		if (config.map.players[owner].group==config.map.players[config.map.player_id].group)
			Color4f(0,0.6,1,1.0);
		else
			Color4f(1,0,0,1.0);
}

void drawMinimap(){
	if (config.map.minimap.enable==0)
		return;

	
	float dsize=MINIMAP_AREA_WIDTH;
	float scale=1;
	int i;
	Color4f(1,1,1,1);
	//glEnable(GL_TEXTURE_2D);
	setTexture(&config.map.minimap.tex);
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f (0.0f, 0.0f);
		glVertex2f(config.options.window.width-dsize-MINIMAP_AREA_SHIFT*2-SCREEN_OFFSET,config.options.window.height-dsize/2-MINIMAP_AREA_SHIFT*2-SCREEN_OFFSET);
		glTexCoord2f (0.0f, 1.0f);
		glVertex2f(config.options.window.width-dsize-MINIMAP_AREA_SHIFT*2-SCREEN_OFFSET,config.options.window.height-SCREEN_OFFSET);
		glTexCoord2f (1.0f, 1.0f);
		glVertex2f(config.options.window.width-SCREEN_OFFSET,config.options.window.height-SCREEN_OFFSET);
		glTexCoord2f (1.0f, 0.0f);
		glVertex2f(config.options.window.width-SCREEN_OFFSET,config.options.window.height-dsize/2-MINIMAP_AREA_SHIFT*2-SCREEN_OFFSET);
	glEnd();
	scale=MINIMAP_SIZE*1.0/config.map.grid_size;
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(config.options.window.width-dsize-SCREEN_OFFSET-1,config.options.window.height-dsize/4-SCREEN_OFFSET-1,0);	
	Color4f(0,0,0,0.8);
	//need to add tex under map 
	glScalef(1,0.5,1);
	glRotatef(-45,0,0,1);
		glBegin(GL_TRIANGLE_FAN);
			glTexCoord2f (0.0f, 0.0f);
			glVertex2f(0.0f,0.0f);
			glTexCoord2f (1.0f, 0.0f);
			glVertex2f(MINIMAP_SIZE,0.0f);
			glTexCoord2f (1.0f, 1.0f);
			glVertex2f(MINIMAP_SIZE,MINIMAP_SIZE);
			glTexCoord2f (0.0f, 1.0f);
			glVertex2f(0.0f,MINIMAP_SIZE);
		glEnd();
		//draw towers
		#define t_size 0.4f
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(1);
		for(i=0;i<config.map.tower_max;i++)
			if (config.map.tower_array[i].id!=0){
				glBegin(config.map.tower_array[i].type!=BASE?GL_LINES:GL_TRIANGLE_FAN);
					//check group and set color
					setMinimapObjectColor(config.map.tower_array[i].owner);
					glVertex2f(scale*(config.map.tower_array[i].position.x+t_size),scale*config.map.tower_array[i].position.y);
					glVertex2f(scale*(config.map.tower_array[i].position.x-t_size),scale*config.map.tower_array[i].position.y);
					glVertex2f(scale*config.map.tower_array[i].position.x,scale*(config.map.tower_array[i].position.y+t_size));
					glVertex2f(scale*config.map.tower_array[i].position.x,scale*(config.map.tower_array[i].position.y-t_size));
				glEnd();
			}
		glDisable(GL_LINE_SMOOTH);
		#undef t_size	
		//draw npcs
		glPointSize(3);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
			for(i=0;i<config.map.npc_max;i++)
				if (config.map.npc_array[i].id!=0){
					//Color4f(0.2,1,0.2,1.0);
					setMinimapObjectColor(config.map.npc_array[i].owner);
					glVertex2f(scale*config.map.npc_array[i].position.x,scale*config.map.npc_array[i].position.y);
				}
		glEnd();
//		glPointSize(1);
//		Color4f(1.0f,0.7f,0,1.0);
//		glBegin(GL_POINTS);
//			for(i=0;i<config.map.bullet_max;i++)
//				if (config.map.bullet_array[i].id!=0)
//					glVertex2f(scale*config.map.bullet_array[i].position.x,scale*config.map.bullet_array[i].position.y);
//		glEnd();
		glPointSize(1);
		glDisable(GL_POINT_SMOOTH);
		//draw minimap grid
		Color4f(0.8,0.8,0.8,0.4);
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(0.7);
		glBegin(GL_LINES);
			for(i=0;i<=config.map.grid_size;i++){
				glVertex2f(scale*i,scale*0);
				glVertex2f(scale*i,scale*config.map.grid_size);
				glVertex2f(scale*0,scale*i);
				glVertex2f(scale*config.map.grid_size,scale*i);
			}
		glEnd();
		glDisable(GL_LINE_SMOOTH);
		//draw screen quad
		Color4f(1,1,1,1);
		glBegin(GL_LINE_LOOP);
			glVertex2f(scale*config.global.screen.coord.ld.x,
					scale*config.global.screen.coord.ld.y);	
			glVertex2f(scale*config.global.screen.coord.lu.x,
					scale*config.global.screen.coord.lu.y);	
			glVertex2f(scale*config.global.screen.coord.ru.x,
					scale*config.global.screen.coord.ru.y);	
			glVertex2f(scale*config.global.screen.coord.rd.x,
					scale*config.global.screen.coord.rd.y);	
		glEnd();
		
	glPopMatrix();
}

void drawPlayerHealth(){
	
}

void drawMessage(){
	if (*config.message==0)
		return;
	glEnable(GL_TEXTURE_2D);
	Color4f(1,1,1,1);
	glPushMatrix();
		glTranslatef(config.options.window.width/2.0,config.options.window.height/2.0+100,0);	
		//glScalef(15,15,1);	
		drawTextCentered(localeTextGet(config.message));
	glPopMatrix();
	if (++config.message_ticks>800){
		config.message_ticks=0;
		*config.message=0;
	}
		
}

void drawFrameTime(){
	char buf[100];//="test";
	sprintf(buf,"fps:%.1f ms/f:%.1f ",(1000/(config.global.frame_time+0.0001)),config.global.frame_time);
	//glScalef(12,12,1); //from glFont1
	glScalef(0.85,0.85,1);
	Color4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glFontTextOut(localeTextGet(buf),2,20,0);
	char2 t[15]={0xdf,0x41f,0x440,0x438,0x432,0x435,0x422,0xdf,'5',0};
	glFontTextOut(t,200,20,0);
}

		
void drawScene(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glClear(GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
//	glEnable(GL_DEPTH_TEST);
	//when we load something

	if (config.loading.enable!=0){
		drawMenu(&config.loading);
		goto out;
	}
	if (config.auth==0){
		drawMenu(&config.auth_menu);
		goto cur;
	}
	if (config.map.enable!=0){
		glPushMatrix();
			globalTransform();
			glDisable(GL_DEPTH_TEST);
			if (config.options.darkness.enable!=0)
				getLightsMask();
			
			drawMap();
		
			config.texture_no_change=1;
			drawWalls();
			drawMapObjects();
			drawNpcs();
			drawTowers();
			drawBullets();
			drawSplashes();
			//draw map
			glEnable(GL_DEPTH_TEST);
			config.texture_no_change--;
			drawWalls();
			drawMapObjects();
			drawNpcs();
			drawTowers();
			drawBullets();
			drawSplashes();
			//draw map egain
		glPopMatrix();
		glDisable(GL_DEPTH_TEST);

		if (config.options.darkness.enable!=0){
			drawLightsMask();
		}
		
		drawMinimap();
	}
	
	//draw screen controls
	glDisable(GL_DEPTH_TEST);
	drawMessage();
	drawMenu(&config.map.screen_menu);
	drawMenu(&config.map.tower_menu);
	drawMenu(&config.map.npc_menu);
	if (config.map.action_menu.enable!=0)
		drawMenu(&config.map.action_menu);
	if (config.menu.enable!=0)
		drawMenu(&config.menu.root);
cur:
	//must be the last
	if (config.text.enable!=0)
		drawMenu(&config.text.keyboard);
	drawCursor(); 
out:	
	//must be the last
	drawFrameTime();
	glFlush();
	SDL_GL_SwapWindow(config.window);
}

