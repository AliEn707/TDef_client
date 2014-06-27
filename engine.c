#include "main.h "
#include "engine.h "




int checkMouseOnObject(object * o){
	if (cursor.state.x>o->position.x && 
			cursor.state.x<o->position.x+o->size.x &&
			cursor.state.y>o->position.y && 
			cursor.state.y<o->position.y+o->size.y)
		return 1;
	return 0;
}


void mouseMotion(){
	cursor.state.x+=(cursor.pos.x-cursor.prev.x)*cursor.sens;
	cursor.state.y+=-(cursor.pos.y-cursor.prev.y)*cursor.sens;
	if (cursor.state.x<0)
		cursor.state.x=0;
	if (cursor.state.y<0)
		cursor.state.y=0;
	if (cursor.state.y>config.window_height)
		cursor.state.y=config.window_height;
	if (cursor.state.x>config.window_width)
		cursor.state.x=config.window_width;
	
	cursor.prev.x=cursor.pos.x;
	cursor.prev.y=cursor.pos.y;
}


int checkMouseState(){
	mouseMotion();
//	printf("%d %d\n",cursor.state.x,cursor.state.y);
	
	
	
	return 0;
}


void cursorInit(){
	cursor.sens=1;
	cursor.color.r=1;
	cursor.color.g=1;
	cursor.color.b=1;
	cursor.state.x=0;
	cursor.state.y=config.window_height;
	
}


int getNewTexture(){
	glGenTextures(1,config.textures+config.textures_size);
	return config.textures[config.textures_size++];
}
#define tx config.transform.translate.x
#define ty config.transform.translate.y
#define sx config.transform.scale
#define sy (sx/2)

//from grid to screen
//  y
//0
//  x

float gridToScreenX(float x, float y){
	
	return sx*(0.707*(x)+0.707*(y))+tx;
}

float gridToScreenY(float x, float y){
	return sy*(0.707*(y)-0.707*(x))+ty;
}

//from screen to grid
//sreen 
//y
//0 x

float screenToGridX(float x, float y){
		return -(500*y)/(707*sy)+(500*x)/(707*sx)+
			(500*sx*ty-500*sy*tx)/
			(707*sx*sy);
}

float screenToGridY(float x, float y){
		return (500*y)/(707*sy)+(500*x)/(707*sx)-
			(500*sx*ty+500*sy*tx)/
			(707*sx*sy);
}
#undef tx
#undef ty
#undef sx
#undef sy



void graficsInit(){

	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ){ 
	 	exit(1);
	} 
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 1);

	config.window = SDL_CreateWindow("Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config.window_width, config.window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	
	SDL_GLContext glcontext = SDL_GL_CreateContext(config.window); // создаем контекст OpenGL
	
	if(config.window == NULL){
		exit(1);
	}
	
	cursorInit();
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,config.window_width,0,config.window_height,-100,100);
	glMatrixMode(GL_MODELVIEW); 
}
