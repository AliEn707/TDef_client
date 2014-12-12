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

void drawText(char2* text,float width){
//	glPushMatrix();		
//	glScalef(15,15,1);
//	glTranslatef(0,glFontHeight(&mainfont,text),0);
	glEnable(GL_TEXTURE_2D);
	glFontTextOut(text,0,0,width);
//	glPopMatrix();
}

void drawTextCentered(char2* text,float width){
	glPushMatrix();		
//	glScalef(15,15,1);
	glTranslatef(-glFontWigth(text)/2,0,0);
	glEnable(GL_TEXTURE_2D);
	glFontTextOut(text,0,0,width);
	glPopMatrix();
}



void drawCursor(){
	Color4f(cursor.color.r,cursor.color.g,cursor.color.b,1);
	glDisable(GL_DEPTH_TEST);
	setTexture(&cursor.tex);
	float cur_size=50;
	Begin(GL_TRIANGLE_FAN);
		Vertex2f(cursor.state.x+cur_size,cursor.state.y-cur_size);
		TexCoord2f(1.0f, 0.0f);
		Vertex2f(cursor.state.x,cursor.state.y-cur_size);
		TexCoord2f(0.0f, 0.0f);
		Vertex2f(cursor.state.x,cursor.state.y);
		TexCoord2f(0.0f, 1.0f);
		Vertex2f(cursor.state.x+cur_size,cursor.state.y);
		TexCoord2f(1.0f, 1.0f);
		
	End();
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
				glTranslatef(cursor.state.x+41-length,cursor.state.y-23,0);
				//glScalef(15,15,1); //need to correct
				//glTranslatef(-length,0,0);
				//glTranslatef(-length/2.0f,height,0);
				Color4f(0.0f, 0.0f, 0.0f, 0.8f);
				Begin(GL_TRIANGLE_FAN);
					Vertex3f(-shift,shift,0);
					TexCoord2f(1.0f, 0.0f);
					Vertex3f(-shift,-height-shift,0);
					TexCoord2f(0.0f, 0.0f);
					Vertex3f(length+shift+1,-height-shift,0);
					TexCoord2f(0.0f, 1.0f);
					Vertex3f(length+shift+1,shift,0);
					TexCoord2f(1.0f, 1.0f);
				End();
					glEnable(GL_TEXTURE_2D);
					Color4f(1.0f, 1.0f, 1.0f,1.0f);
					drawText(cur_text,0);
			glPopMatrix();
		}
}

static int current_tex;
int setTexture(texture * t){
	if (t==0){
		current_tex=0;
		return 0;
	}
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
	if (current_tex!=t->tex[(int)t->current_frame]){
		glBindTexture(GL_TEXTURE_2D, t->tex[(int)t->current_frame]);
		current_tex=t->tex[(int)t->current_frame];
	}// else printf("no tex change\n");
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
	
	Begin(GL_TRIANGLE_FAN);
		TexCoord2f(0.0f, 0.0f);
		Vertex2f(0,0);
		TexCoord2f(0.0f, tiles.y);
		Vertex2f(0,e->size.y);
		TexCoord2f(tiles.x, tiles.y);
		Vertex2f(e->size.x,e->size.y);
		TexCoord2f(tiles.x, 0.0f);
		Vertex2f(e->size.x,0);
	End();
	
texbreak:
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
	Begin(GL_LINE_LOOP);
		TexCoord2f(0.0f, 0.0f);
		Vertex2f(0,0);
		TexCoord2f(0.0f, 1.0f);
		Vertex2f(0,e->size.y);
		TexCoord2f(1.0f, 1.0f);
		Vertex2f(e->size.x,e->size.y);
		TexCoord2f(1.0f, 0.0f);
		Vertex2f(e->size.x,0);
	End();
	
wirebreak:	
	glEnable(GL_TEXTURE_2D);
	if (*e->text==0)
		goto exit;
		//draw text
		//Color4f(1,1,1,1);
	char2 * text=localeTextGet(e->text);
	glTranslatef(e->text_position.x,
				e->text_position.y,
				0);
	
		if (e->text_size!=0)
			glScalef(e->text_size,e->text_size,1);
		glTranslatef(0,glFontHeight(text),0);
		if (e->text_centered!=0)
			drawTextCentered(text,e->size.x-e->text_position.x*2);
		else
			drawText(text,e->size.x-e->text_position.x*2);
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
		for(i=0;i<o->$elements;i++)
			drawElement(&o->elements[i],o->in_focus);
	glPopMatrix();
}

int drawMenu(menu * root){
	menu* m=0;
	m=root;
	int i;
	glDisable(GL_TEXTURE_2D);
	for(i=0;i<config.menu.depth;i++)
		m=&m->submenu[config.menu.path[i]];
	for(i=0;i<m->$objects;i++)
		drawObject(&m->objects[i]);
	return 0;
}

static inline void drawNode(gnode * n) __attribute__((always_inline));
static inline void drawNode(gnode * n){
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
//		Begin(GL_LINE_LOOP);
		Begin(GL_TRIANGLE_FAN);
			TexCoord2f (0.0f, 0.0f);
			Vertex2f(_x,_y);
			TexCoord2f (1.0f, 0.0f);
			Vertex2f(_x+1.0f,_y);
			TexCoord2f (1.0f, 1.0f);
			Vertex2f(_x+1.0f,_y+1.0f);
			TexCoord2f (0.0f, 1.0f);
			Vertex2f(_x,_y+1.0f);
		End();
		if (n->buildable!=0){
			setTexture(&config.map.tex[BUILDABLE]);
			Begin(GL_TRIANGLE_FAN);
				TexCoord2f (0.0f, 0.0f);
				Vertex2f(_x,_y);
				TexCoord2f (1.0f, 0.0f);
				Vertex2f(_x+1.0f,_y);
				TexCoord2f (1.0f, 1.0f);
				Vertex2f(_x+1.0f,_y+1.0f);
				TexCoord2f (0.0f, 1.0f);
				Vertex2f(_x,_y+1.0f);
			End();
		}
		if (config.map.show_walk!=0){
			Color4f(1,1,1,1);
			if (n->walkable<1){
				if (n->walkable==0)
					setTexture(&config.map.tex[WALKABLE]);
				else
					setTexture(&config.map.tex[NO_SEE]);
				Begin(GL_TRIANGLE_FAN);
					TexCoord2f (0.0f, 0.0f);
					Vertex2f(_x,_y);
					TexCoord2f (1.0f, 0.0f);
					Vertex2f(_x+1.0f,_y);
					TexCoord2f (1.0f, 1.0f);
					Vertex2f(_x+1.0f,_y+1.0f);
					TexCoord2f (0.0f, 1.0f);
					Vertex2f(_x,_y+1.0f);
				End();
			}
		}
//	glPopMatrix();	
//	glDisable(GL_TEXTURE_2D);
}


static inline void drawMapGrid() __attribute__((always_inline));
static inline void drawMapGrid(){
	int i,j;
	for(i=0;i<config.map.grid_size;i++)
		for(j=0;j<config.map.grid_size;j++)
			if (checkGridLines(i,j))
				drawNode(&config.map.grid[to2d(i,j)]);
	//draw non working zone
	int k;
	k=0;
	for(i=-1;i>-(config.map.grid_size/2+config.map.grid_size%2+1);i--)
		for(j=-i-1;j<config.map.grid_size-(-i-1);j++){
			if (checkGridLines(i,j)){
				glPushMatrix();
					glTranslatef(i,j,0);
					drawNode(&config.map.grid_out[0][k]);
				glPopMatrix();
			}
			k++;
		}
	k=0;
	for(j=0;j<(config.map.grid_size/2+config.map.grid_size%2+1);j++)
		for(i=j;i<config.map.grid_size-j;i++){
			if (checkGridLines(i,config.map.grid_size+j)){
				glPushMatrix();
					glTranslatef(i,config.map.grid_size+j,0);
					drawNode(&config.map.grid_out[1][k]);
				glPopMatrix();
			}
			k++;
		}
	k=0;
	for(i=0;i<(config.map.grid_size/2+config.map.grid_size%2+1);i++)
		for(j=i;j<config.map.grid_size-i;j++){
			if (checkGridLines(config.map.grid_size+i,j)){
				glPushMatrix();
					glTranslatef(config.map.grid_size+i,j,0);
					drawNode(&config.map.grid_out[2][k]);
				glPopMatrix();
			}
			k++;
		}
	k=0;
	for(j=-1;j>-(config.map.grid_size/2+config.map.grid_size%2+1);j--)
		for(i=-j-1;i<config.map.grid_size-(-j-1);i++){
			if (checkGridLines(i,j)){
				glPushMatrix();
					glTranslatef(i,j,0);
					drawNode(&config.map.grid_out[3][k]);
				glPopMatrix();
			}
			k++;
		}
}

inline void globalTransform(){
	glTranslatef(config.global.transform.translate.x,config.global.transform.translate.y,-100);
	glScalef(config.global.transform.scale,config.global.transform.scale,1);
	glRotatef(-60,1,0,0);
	//glScalef(1,0.5,1);
	glRotatef(-45,0,0,1);
	
}

inline void backTransform(){
	glRotatef(45,0,0,1);

	glRotatef(60,1,0,0);
// try to set up
//	glRotatef(90,1,0,0);
//	glScalef(1,1.154,1);

//	glScalef(1,2,1);
	//glScalef(1,4*1,1);
}

#define add (size.y*0.4f)
void drawHealth(vec2 pos,vec2 size,float p,float s,int shield){
	float $paper=size.y;
	if (shield==0)
		$paper=0;
//	glPushMatrix();
//	glTranslatef(0,0,0.03);//why?
//	glDisable(GL_TEXTURE_2D);
	setTexture(&config.map.tex[HEALTH_PAPER]);
	Color4f(1,1,1,1);
	Begin(GL_TRIANGLE_FAN);
		TexCoord2f (0, 0.0f);
		Vertex2f(pos.x-add,pos.y-add-$paper);
		TexCoord2f (1.0f, 0.0f);
		Vertex2f(pos.x+size.x+add,pos.y-add-$paper);
		TexCoord2f (1.0f, 1.0f);
		Vertex2f(pos.x+size.x+add,pos.y+size.y+add);
		TexCoord2f (0, 1.0f);
		Vertex2f(pos.x-add,pos.y+size.y+add);
	End();
	setTexture(&config.map.tex[HEALTH]);
	//health
	Color4f(1.2*(1-p),0.9*p,0,1);
	Begin(GL_TRIANGLE_FAN);
		TexCoord2f (0.0f, 0.0f);
		Vertex2f(pos.x,pos.y);
		TexCoord2f (p, 0.0f);
		Vertex2f(pos.x+size.x*p,pos.y);
		TexCoord2f (p, 1.0f);
		Vertex2f(pos.x+size.x*p,pos.y+size.y);
		TexCoord2f (0.0f, 1.0f);
		Vertex2f(pos.x,pos.y+size.y);
	End();
	Color4f(0,0,0,0.8);
	Begin(GL_TRIANGLE_FAN);
		TexCoord2f (p, 0.0f);
		Vertex2f(pos.x+size.x*p,pos.y);
		TexCoord2f (1.0f, 0.0f);
		Vertex2f(pos.x+size.x,pos.y);
		TexCoord2f (1.0f, 1.0f);
		Vertex2f(pos.x+size.x,pos.y+size.y);
		TexCoord2f (p, 1.0f);
		Vertex2f(pos.x+size.x*p,pos.y+size.y);
	End();
	if (shield!=0){
		//shield
//		Color4f(0,0.75,1,1);
		Begin(GL_TRIANGLE_FAN);
			TexCoord2f (s, 0.0f);
			Vertex2f(pos.x+size.x*s,pos.y-size.y);
			TexCoord2f (1.0f, 0.0f);
			Vertex2f(pos.x+size.x,pos.y-size.y);
			TexCoord2f (1.0f, 1.0f);
			Vertex2f(pos.x+size.x,pos.y);
			TexCoord2f (s, 1.0f);
			Vertex2f(pos.x+size.x*s,pos.y);
		End();
		Color4f(0.27,0.51,0.71,1);
		Begin(GL_TRIANGLE_FAN);
			TexCoord2f (0.0f, 0.0f);
			Vertex2f(pos.x,pos.y-size.y);
			TexCoord2f (s, 0.0f);
			Vertex2f(pos.x+size.x*s,pos.y-size.y);
			TexCoord2f (s, 1.0f);
			Vertex2f(pos.x+size.x*s,pos.y);
			TexCoord2f (0.0f, 1.0f);
			Vertex2f(pos.x,pos.y);
		End();
	}
//	glPopMatrix();
}
#undef add

static inline void drawNpc(npc* n) __attribute__((always_inline));
static inline void drawNpc(npc* n){
	float size=0.89f;
	npc_type * type=typesNpcGet(n->type);
	if (type==0)
		return;
	if (type->t_size!=0)
		size*=type->t_size;
	glPushMatrix();
	glTranslatef(n->position.x,n->position.y,0);
	backTransform();
//	printf("n->type - %d\n",n->type);
	glScalef(size,size,1);
//	glTranslatef(0,0,.56f);
//	glTranslatef(0,0,.3f);
		if (n->tex[n->current_tex].frames==0){
			if (type->tex[n->current_tex].frames==0)
				//npc stored in global tex memory
				loadTexture(&type->tex[n->current_tex],type->tex_path[n->current_tex]);
			memcpy(&n->tex[n->current_tex],&type->tex[n->current_tex],sizeof(texture));
		}
		if(setTexture(&n->tex[n->current_tex]))
			n->anim_ended=1;
//		else
//			n->anim_ended=0;
		Color4f(1,1,1,1);
//		Begin(GL_LINE_LOOP);
		Begin(GL_TRIANGLE_FAN);
			TexCoord2f (0.005f, 0.005f);
			Vertex2f(-0.5f,0.0f);
			TexCoord2f (0.995f, 0.005f);
			Vertex2f(0.5f,0.0f);
			TexCoord2f (0.995f, 0.995f);
			Vertex2f(0.5f,1.0f);
			TexCoord2f (0.005f, 0.995f);
			Vertex2f(-0.5f,1.0f);
		End();
		
//		glTranslatef(0,0,0.1169);
		glTranslatef(0,0,5);
		//draw health
		float health=1.0*n->health/type->health;
		float shield=type->shield?1.0*n->shield/type->shield:0;
		if (health<0.98 || shield<0.98)
			drawHealth((vec2){-0.5,0.9},(vec2){0.9,0.05},health,shield,type->shield);
//		glPushMatrix();
			Color4f(1,1,1,1);
			glTranslatef(-0.5,0.9,0);
			//glScalef(0.15,0.15,1);
			glScalef(0.011,0.011,1);
			char buf[5];
			sprintf(buf,"%hd",n->level);
			drawText(localeTextGet(buf),0);
//		glPopMatrix();
		
	glPopMatrix();
}

static inline void drawNpcs() __attribute__((always_inline));
static inline void drawNpcs(){
	int i;
	for(i=0;i<config.map.npc_max;i++)
		if (config.map.npc_array[i].id!=0)
			if (checkGridLines(config.map.npc_array[i].position.x,config.map.npc_array[i].position.y)){
//				drawNpc(&config.map.npc_array[i]);
				hashAdd(config.map.npc_array[i].position.x+config.map.grid_size-config.map.npc_array[i].position.y,&config.map.npc_array[i]);
			}
}

static inline void drawTower(tower* t) __attribute__((always_inline));
static inline void drawTower(tower* t){
	float size=1.4f;
	tower_type * type=typesTowerGet(t->type);
	if (type==0)
		return;
	if (type->t_size!=0)
		size*=type->t_size;
	glPushMatrix();
		glTranslatef(t->position.x,t->position.y,0);
		backTransform();
		glScalef(size,size,1);
//		glTranslatef(0,0.21,.56f);
		glTranslatef(0,0.21,0.0f);
//		printf("tower %d health %d on %d\n",t->id,t->health,posToId(t->position));
		if (t->tex[t->current_tex].frames==0){
			if (type->tex[t->current_tex].frames==0)
				//npc stored in global tex memory
				loadTexture(&type->tex[t->current_tex],type->tex_path[t->current_tex]);
			memcpy(&t->tex[t->current_tex],&type->tex[t->current_tex],sizeof(texture));
		}
		if(setTexture(&t->tex[t->current_tex]))
			t->anim_ended=1;
		
			Color4f(1,1,1,1);
//			Begin(GL_LINE_LOOP);
			Begin(GL_TRIANGLE_FAN);
				TexCoord2f (0.005f, 0.005f);
				Vertex2f(-0.5f,-0.5f);
				TexCoord2f (0.995f, 0.005f);
				Vertex2f(0.5f,-0.5f);
				TexCoord2f (0.995f, 0.995f);
				Vertex2f(0.5f,0.5f);
				TexCoord2f (0.005f, 0.995f);
				Vertex2f(-0.5f,0.5f);
			End();
			//draw level and health
			glTranslatef(0,0,5);//need to draw over all
			float health;
			float shield;
			if (t->type==BASE){
				health=1.0*t->health/config.map.players[t->owner].base_type.health;
				health=config.map.players[t->owner].base_type.shield?1.0*t->shield/config.map.players[t->owner].base_type.shield:0;
			}else{
				health=1.0*t->health/type->health;
				shield=type->shield?1.0*t->shield/type->shield:0;
		
			}
			if (health<0.95 || shield<0.95)
				drawHealth((vec2){-0.325,0.5},(vec2){0.75,0.035},health,shield,type->shield);
			Color4f(1,1,1,1);
//		glPushMatrix();
			glTranslatef(-0.325,0.5,0);
			//glScalef(0.1125,0.1125,1);
			glScalef(0.0075,0.0075,1);
			char buf[5];
			sprintf(buf,"%d",t->level);
			drawText(localeTextGet(buf),0);
//		glPopMatrix();
	glPopMatrix();
}

static inline void drawTowers() __attribute__((always_inline));
static inline void drawTowers(){
	int i;
	for(i=0;i<config.map.tower_max;i++)
		if (config.map.tower_array[i].id!=0)
			if (checkGridLines(config.map.tower_array[i].position.x,config.map.tower_array[i].position.y)){
//				drawTower(&config.map.tower_array[i]);
				hashAdd(config.map.tower_array[i].position.x+config.map.grid_size-config.map.tower_array[i].position.y,&config.map.tower_array[i]);
			}
}

static inline void drawBullet(bullet* b) __attribute__((always_inline));
static inline void drawBullet(bullet* b){
	
//	printf("%g %g| %g %g %d\n",b->position.x,b->position.y,b->direction.x,b->direction.y,b->type);
	vec2* pos;
	float length=1;
//	type->solid=1;
	bullet_type * type=typesBulletGet(b->type);
	if (type==0)
		return;
	if (type->solid!=0)
		pos=&b->source;
	else
		pos=&b->position;
	
	if (b->destination.x==pos->x && b->destination.y==pos->y)
		pos=&b->source;
	glPushMatrix();
	glTranslatef(pos->x,pos->y,0);
	backTransform();
//	glTranslatef(0,0.2,1.01);
	glTranslatef(0,0.2,0.01);
	float x;
	
//	else
	{
		float ang;
		vec2 dir={b->destination.x-pos->x,b->destination.y-pos->y};
		length=sqrtf(sqr(dir.x)+sqr(dir.y));
		
		float y;
		x=gridToScreenX(b->destination.x,b->destination.y);
		y=gridToScreenY(b->destination.x,b->destination.y);
		x-=gridToScreenX(pos->x,pos->y);
		y-=gridToScreenY(pos->x,pos->y);
//		printf("%g %g| %g %g\n",dir.x,b->direction.x,dir.y,b->direction.y);
		if (type->solid==0){
			if (sign(b->direction.x)!=sign(dir.x))
				goto drawBulletExit;
	//			x*=-1;
			if (sign(b->direction.y)!=sign(dir.y))
				goto drawBulletExit;
		}
//			y*=-1;
//		if (x==0 )
//			goto drawBulletExit;
		
		float l=sqrtf(sqr(x)+sqr(y));
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
		if (type->tex[b->current_tex].frames==0)
			loadTexture(&type->tex[b->current_tex],type->tex_path[b->current_tex]);
		memcpy(&b->tex[b->current_tex],&type->tex[b->current_tex],sizeof(texture));
	}
	float tx1,ty1,tx2,ty2,vx;
	float height=0.08f;
	if (type->solid==0){
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
//		Begin(GL_LINE_LOOP);
		Begin(GL_TRIANGLE_FAN);
			TexCoord2f (tx1, ty1);
			Vertex2f(0.0f,-height);
			TexCoord2f (tx1, ty2);
			Vertex2f(0.0f,height);
			TexCoord2f (tx2, ty2);
			Vertex2f(vx,height);
			TexCoord2f (tx2, ty1);
			Vertex2f(vx,-height);
			//texture 1:2
			//texture solid 1:4
			
		End();
drawBulletExit:		
	glPopMatrix();
}

static inline void drawBullets() __attribute__((always_inline));
static inline void drawBullets(){
	int i;
	bullet_type * type;
//	#pragma omp parallel for
	for(i=0;i<config.map.bullet_max;i++)
		if (config.map.bullet_array[i].id!=0){
			type=typesBulletGet(config.map.bullet_array[i].type);
			if (type->solid!=0 || 
					checkGridLines(config.map.bullet_array[i].position.x,config.map.bullet_array[i].position.y)){
//				drawBullet(&config.map.bullet_array[i]);
				hashAdd(type->solid!=0?
							max(config.map.bullet_array[i].position.x+config.map.grid_size-config.map.bullet_array[i].position.y,
									config.map.bullet_array[i].source.x+config.map.grid_size-config.map.bullet_array[i].source.y):
						config.map.bullet_array[i].position.x+config.map.grid_size-config.map.bullet_array[i].position.y,
						&config.map.bullet_array[i]);
			}
		}
}


static inline void drawSplash(splash* s) __attribute__((always_inline));
static inline void drawSplash(splash* s){
	splash_type * type=typesSplashGet(s->type);
	if (type==0)
		return;
	glPushMatrix();
	glTranslatef(s->position.x,s->position.y,0);
	backTransform();
//	glScalef(1.2,1.2,1);
//	glTranslatef(0,0,1);
//	printf("tower %d health %d on %d\n",t->id,t->health,posToId(t->position));
	if (s->tex.frames==0){
		if (type->tex.frames==0)
			loadTexture(&type->tex,type->tex_path);
		memcpy(&s->tex,&type->tex,sizeof(texture));
	}
	if(setTexture(&s->tex))
		s->anim_ended=1;
	
		Color4f(1,1,1,1);
//		Begin(GL_LINE_LOOP);
		Begin(GL_TRIANGLE_FAN);
			TexCoord2f (0.0f, 0.0f);
			Vertex2f(-0.5f,0.0f);
			TexCoord2f (1.0f, 0.0f);
			Vertex2f(0.5f,0.0f);
			TexCoord2f (1.0f, 1.0f);
			Vertex2f(0.5f,1.0f);
			TexCoord2f (0.0f, 1.0f);
			Vertex2f(-0.5f,1.0f);
			
		End();
	glPopMatrix();
}

static inline void drawSplashes() __attribute__((always_inline));
static inline void drawSplashes(){
	int i;
	for(i=0;i<config.map.splash_max;i++)
		if (config.map.splash_array[i].type!=0)
			if (checkGridLines(config.map.splash_array[i].position.x,config.map.splash_array[i].position.y)){
//				drawSplash(&config.map.splash_array[i]);
				hashAdd(config.map.splash_array[i].position.x+config.map.grid_size-config.map.splash_array[i].position.y,&config.map.splash_array[i]);
			}
}

static inline void drawWall(wall* w) __attribute__((always_inline));
static inline void drawWall(wall* w){
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
//		Begin(GL_LINE_LOOP);
		Begin(GL_TRIANGLE_FAN);
			TexCoord2f (0.005f, 0.005f);
			Vertex3f(-x,y,0);
			TexCoord2f (0.995f, 0.005f);
			Vertex3f(x,-y,0);
			TexCoord2f (0.995f, 0.995f);
			Vertex3f(x,-y,2*size);
			TexCoord2f (0.005f, 0.995f);
			Vertex3f(-x,y,2*size);
		End();
	glPopMatrix();
	#undef size
}

static inline void drawWalls() __attribute__((always_inline));
static inline void drawWalls(){
	int i;
	float x;
	float y;
	for(i=0;i<config.map.walls_size;i++)
		if (config.map.walls[i].$$$!=0){
			x=getGridX(config.map.walls[i].position);
			y=getGridY(config.map.walls[i].position);
			if (checkGridLines(x,y)){
//				drawWall(&config.map.walls[i]);
				hashAdd(x-y+config.map.grid_size,&config.map.walls[i]);
			}
		}
}

static inline void drawMapObject(map_object* o) __attribute__((always_inline));
static inline void drawMapObject(map_object* o){
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
//		Begin(GL_LINE_LOOP);
		Begin(GL_TRIANGLE_FAN);
			TexCoord2f (0.0f, 0.0f);
			Vertex2f(-0.5f,0.0f);
			TexCoord2f (1.0f, 0.0f);
			Vertex2f(0.5f,0.0f);
			TexCoord2f (1.0f, 1.0f);
			Vertex2f(0.5f,1.0f);
			TexCoord2f (0.0f, 1.0f);
			Vertex2f(-0.5f,1.0f);
		End();
	glPopMatrix();
}

static inline void drawMapObjects() __attribute__((always_inline));
static inline void drawMapObjects(){
	int i;
	for(i=0;i<config.map.map_$objects;i++)
		if (config.map.map_objects[i].tex!=0){
//			drawMapObject(&config.map.map_objects[i]);
			hashAdd(config.map.map_objects[i].position.x+config.map.grid_size-config.map.map_objects[i].position.y,&config.map.map_objects[i]);
		}
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
	Begin(GL_TRIANGLE_FAN);
		TexCoord2f (0.0f, 0.0f);
		Vertex2f(config.options.window.width-dsize-MINIMAP_AREA_SHIFT*2-SCREEN_OFFSET,config.options.window.height-dsize/2-MINIMAP_AREA_SHIFT*2-SCREEN_OFFSET);
		TexCoord2f (0.0f, 1.0f);
		Vertex2f(config.options.window.width-dsize-MINIMAP_AREA_SHIFT*2-SCREEN_OFFSET,config.options.window.height-SCREEN_OFFSET);
		TexCoord2f (1.0f, 1.0f);
		Vertex2f(config.options.window.width-SCREEN_OFFSET,config.options.window.height-SCREEN_OFFSET);
		TexCoord2f (1.0f, 0.0f);
		Vertex2f(config.options.window.width-SCREEN_OFFSET,config.options.window.height-dsize/2-MINIMAP_AREA_SHIFT*2-SCREEN_OFFSET);
	End();
	scale=MINIMAP_SIZE*1.0/config.map.grid_size;
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(config.options.window.width-dsize-SCREEN_OFFSET-1,config.options.window.height-dsize/4-SCREEN_OFFSET-1,0);	
	Color4f(0,0,0,0.8);
	//need to add tex under map 
	glScalef(1,0.5,1);
	glRotatef(-45,0,0,1);
		Begin(GL_TRIANGLE_FAN);
			TexCoord2f (0.0f, 0.0f);
			Vertex2f(0.0f,0.0f);
			TexCoord2f (1.0f, 0.0f);
			Vertex2f(MINIMAP_SIZE,0.0f);
			TexCoord2f (1.0f, 1.0f);
			Vertex2f(MINIMAP_SIZE,MINIMAP_SIZE);
			TexCoord2f (0.0f, 1.0f);
			Vertex2f(0.0f,MINIMAP_SIZE);
		End();
		//draw towers
		#define t_size 0.4f
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(1);
		for(i=0;i<config.map.tower_max;i++)
			if (config.map.tower_array[i].id!=0){
				setMinimapObjectColor(config.map.tower_array[i].owner);
				Begin(config.map.tower_array[i].type!=BASE?GL_LINES:GL_TRIANGLE_FAN);
					//check group and set color
					TexCoord2f (0.0f, 0.0f);
					Vertex2f(scale*(config.map.tower_array[i].position.x+t_size),scale*config.map.tower_array[i].position.y);
					TexCoord2f (1.0f, 0.0f);
					Vertex2f(scale*(config.map.tower_array[i].position.x-t_size),scale*config.map.tower_array[i].position.y);
					TexCoord2f (1.0f, 1.0f);
					Vertex2f(scale*config.map.tower_array[i].position.x,scale*(config.map.tower_array[i].position.y+t_size));
					TexCoord2f (0.0f, 1.0f);
					Vertex2f(scale*config.map.tower_array[i].position.x,scale*(config.map.tower_array[i].position.y-t_size));
				End();
			}
		glDisable(GL_LINE_SMOOTH);
		#undef t_size	
		//draw npcs
		glPointSize(3);
		glEnable(GL_POINT_SMOOTH);
			for(i=0;i<config.map.npc_max;i++)
				if (config.map.npc_array[i].id!=0){
					//Color4f(0.2,1,0.2,1.0);
					setMinimapObjectColor(config.map.npc_array[i].owner);
					Begin(GL_POINTS);
						TexCoord2f (0.0f, 0.0f);
						Vertex2f(scale*config.map.npc_array[i].position.x,scale*config.map.npc_array[i].position.y);
					End();
				}
		glPointSize(1);
//		Color4f(1.0f,0.7f,0,1.0);
//		Begin(GL_POINTS);
//			for(i=0;i<config.map.bullet_max;i++)
//				if (config.map.bullet_array[i].id!=0)
//					Vertex2f(scale*config.map.bullet_array[i].position.x,scale*config.map.bullet_array[i].position.y);
//		End();
		glPointSize(1);
		glDisable(GL_POINT_SMOOTH);
		//draw minimap grid
		Color4f(0.8,0.8,0.8,0.4);
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(0.7);
		Begin(GL_LINES);
			for(i=0;i<=config.map.grid_size;i++){
				TexCoord2f (0.0f, 0.0f);
				Vertex2f(scale*i,scale*0);
				TexCoord2f (0.0f, 1.0f);
				Vertex2f(scale*i,scale*config.map.grid_size);
				TexCoord2f (1.0f, 1.0f);
				Vertex2f(scale*0,scale*i);
				TexCoord2f (0.0f, 1.0f);
				Vertex2f(scale*config.map.grid_size,scale*i);
			}
		End();
		glDisable(GL_LINE_SMOOTH);
		//draw screen quad
		Color4f(1,1,1,1);
		Begin(GL_LINE_LOOP);
			TexCoord2f (0.0f, 0.0f);
			Vertex2f(scale*config.global.screen.coord.ld.x,
					scale*config.global.screen.coord.ld.y);	
			TexCoord2f (1.0f, 0.0f);
			Vertex2f(scale*config.global.screen.coord.lu.x,
					scale*config.global.screen.coord.lu.y);	
			TexCoord2f (1.0f, 1.0f);
			Vertex2f(scale*config.global.screen.coord.ru.x,
					scale*config.global.screen.coord.ru.y);	
			TexCoord2f (0.0f, 1.0f);
			Vertex2f(scale*config.global.screen.coord.rd.x,
					scale*config.global.screen.coord.rd.y);	
		End();
		
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
		drawTextCentered(localeTextGet(config.message),0);
	glPopMatrix();
	if (++config.message_ticks>800){
		config.message_ticks=0;
		*config.message=0;
	}
		
}

static inline void drawFrameTime() __attribute__((always_inline));
static inline void drawFrameTime(){
	char buf[100];//="test";
	sprintf(buf,"fps:%.1f ms/f:%.1f ",(1000/(config.global.frame_time+0.0001)),config.global.frame_time);
	//glScalef(12,12,1); //from glFont1
	glScalef(0.85,0.85,1);
	Color4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glFontTextOut(localeTextGet(buf),2,20,0);
	///
	char2 t[15]={0xdf,0x41f,0x440,0x438,0x432,0x435,0x422,0xdf,'5',0};
	glFontTextOut(t,200,20,0);
}

void drawHash(void * c_c){
	int i_i=*((char*)c_c);
	switch(i_i){
		case STRUCT_NPC:
			drawNpc(c_c);
			break;
		case STRUCT_TOWER:
			drawTower(c_c);
			break;
		case STRUCT_BULLET:
			drawBullet(c_c);
			break;
		case STRUCT_MAPOBJECT:
			drawMapObject(c_c);
			break;
		case STRUCT_SPLASH:
			drawSplash(c_c);
			break;
		case STRUCT_WALL:
			drawWall(c_c);
			break;
	}
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
			//viewport in borders, need for antialiasing
			glViewport(SCREEN_OFFSET, SCREEN_OFFSET, config.options.window.width-SCREEN_OFFSET*2, config.options.window.height-SCREEN_OFFSET*2);
			
			drawMapGrid();
//			morefast way 		
//			config.texture_no_change=1;
//			drawWalls();
//			drawMapObjects();
//			drawNpcs();
//			drawTowers();
//			drawBullets();
//			drawSplashes();
			//draw map
//			glEnable(GL_DEPTH_TEST);
//			config.texture_no_change--;
			drawWalls();
			drawMapObjects();
			drawNpcs();
			drawTowers();
			drawBullets();
			drawSplashes();
			hashForEach(drawHash);
			//draw map egain
		glPopMatrix();
//		glDisable(GL_DEPTH_TEST);

		if (config.options.darkness.enable!=0){
			drawLightsMask();
		}
		glViewport(0, 0, config.options.window.width, config.options.window.height);
		
		drawMinimap();
	}
	
	glDisable(GL_DEPTH_TEST);
	
	//draw screen controls
	if (config.public.enable!=0){
		publicDraw();
	}
	if (config.map.enable!=0){
		drawMenuMap();
	}
	if (config.menu.enable!=0)
		drawMenu(&config.menu.root);
cur:
	//must be the last
	if (config.text.enable!=0)
		if (config.text.keyboard.enable!=0)
			drawMenu(&config.text.keyboard);
	drawCursor(); 
	drawMessage();
out:	
	//must be the last
	drawFrameTime();
	glFlush();
	SDL_GL_SwapWindow(config.window);
}

