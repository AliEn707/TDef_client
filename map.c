#include "main.h "
#include "map.h "


//

void checkMouseMap(){
	float x=screenToGridX(cursor.state.x,cursor.state.y);
	float y=screenToGridY(cursor.state.x,cursor.state.y);
	config.map.focus=to2di(x,y);
}

void processKeysMap(SDL_Event event){
	
}


void processContKeysMap(){
	if (config.global.keys[SDLK_w]!=0)
		setMove(-2,0);
	if (config.global.keys[SDLK_s]!=0)
		setMove(2,0);
	if (config.global.keys[SDLK_d]!=0)
		setMove(0,-2);
	if (config.global.keys[SDLK_a]!=0)
		setMove(0,2);
	if (config.global.keys[SDLK_z]!=0)
		setZoom(0.8);
	if (config.global.keys[SDLK_x]!=0)
		setZoom(-0.8);
	//add another
}


int loadMapTexture(char * path){
//	config.textures[config.textures_size]=loadTGATexture(path);
//	return config.textures[config.textures_size++];
}

void globalTransformCorrection(){
	float xl=gridToScreenX(0,0);
	float xr=gridToScreenX(config.map.grid_size,config.map.grid_size);
	float yd=gridToScreenY(config.map.grid_size,0);
	float yu=gridToScreenY(0,config.map.grid_size);
	if (xl>0)
		config.map.transform.translate.x-=xl;
	if (xr<config.window_width)
		config.map.transform.translate.x-=xr-config.window_width;
	if (yu<config.window_height)
		config.map.transform.translate.y-=yu-config.window_height;
	if (yd>0)
		config.map.transform.translate.y-=yd;
	float x=xr-xl;
	float y=yu-yd;
	float scale=1;
	if (x<config.window_width || y<config.window_height)
		if (x-config.window_width<y-config.window_height)
			scale=config.window_width/x;
		else
			scale=config.window_height/y;
	config.map.transform.scale*=scale;
	
}

void setDefaultTransform(){
	config.map.transform.scale=100;
	globalTransformCorrection();
}



void setMove(float x,float y){
	config.map.transform.translate.x+=y; //do not change!
	config.map.transform.translate.y+=x;
	globalTransformCorrection();
}

void setZoom(float x){
	config.map.transform.scale+=x;
	if (config.map.transform.scale<=0)
		config.map.transform.scale=0.2;
	globalTransformCorrection();
}


#define tx config.map.transform.translate.x
#define ty config.map.transform.translate.y
#define sx config.map.transform.scale
#define sy (sx/2)

//take to map.c file

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

