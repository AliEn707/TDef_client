//#include <windows.h> 
#include <time.h> 

#include "headers.h"
/*
#include "../main.h"
#include "../locale.h"
#include "../tga.h"
#include "../drawer.h"
#include "../engine.h"
#include "../file.h"
#include "../menu.h"
#include "../map.h"
#include "../threads.h"
#include "../network.h"
#include "../lights.h"
#include "../public.h"
*/




void drawCube(float xrf, float yrf, float zrf);



#undef main
//int main(int argc, char *argv[]){   
int main(){   
	printf("build at %s %s:%d\n",__DATE__,__FILE__,__LINE__);
	printf("Initializing.....");
	memset(&config,0,sizeof(config));
	config.time_per_tick=1000/30;
	config.options.window.width=800;
	config.options.window.height=600;
	config.time_per_frame=1000/60;//60;
	CAMERA_SPEED=8;
	CAMERA_ZOOM=4;
	CURSOR_SPEED=9;
	config.options.tex_filter_mag=GL_LINEAR; // GL_NEAREST
	config.options.tex_filter_min=GL_LINEAR; // GL_NEAREST
	config.options.tex_filter_min=GL_LINEAR_MIPMAP_LINEAR;
	//night mode switch
	config.options.darkness.enable=0;
	config.options.darkness.tex_size=64;
	//brightness 
	config.options.brightness=0.7;
	//color add to screen colors
	config.options.color.r=1;
	config.options.color.g=1;
	config.options.color.b=1;
	//tex quality
	config.options.tex_quality=TEX_MAX; //TEX_MAX
	config.global.latency=8;//must be this
	//screen keyboard
	config.text.keyboard.enable=0; //not need yet
	
	cursor.text="Hello";
	
	graficsInit(); // инициализация
	config.main_running=1;
	printf("done\n");
	
	srand(time(0));
	loadFiles();
	
	
//	FreeConsole();
	
/*	
	printf("locale set %s\n",setlocale( LC_ALL, "ru_ru.65001" ));
	wchar_t wstr[100]=L"\0";
	char str[100]="\0";
	printf("1 %d\n",wcstombs(str,L"привет",100));
	printf("%d\n",strlen(str));
	printf("%s\n",str);
	printf("2 %d\n",mbstowcs(wstr,"привет",100));
	printf("%d\n",wcslen(wstr));
	printf("%ls\n",wstr);
*/	
	
	
	
//	SetConsoleOutputCP(CP_UTF8);
//	printf("%ls\n",L"ПРИвет");
//	float xrf = 0, yrf = 0, zrf = 0; // углы поворота
	Uint32 time=0;

	
	networkInit();
	config.drawer=drawerStart();
	
	config.map.minimap.obj.touch=1;
	config.map.minimap.obj.action=actionMinimap;
	
	sprintf(config.message,"dont worry, its only test");
	while(config.main_running){ 
		workSync(&time);
		Df=TEX_FPS/(1000.0f/(config.global.frame_time+0.001));
		config.global_count++;
//		SDL_Delay(100);
		SDL_Event event;
//		SDL_GetMouseState(&cursor.pos.x,&cursor.pos.y);
		processMouse();
		while( SDL_PollEvent(&event) ){
			processEvent(event);
		}
		processKeyboard();
		if (config.auth!=0)
			if (config.map.enable==0 ){
//				mapStart("public");
//				config.map.enable=1;
//				publicStart();
				config.public.enable=1;
				//add connnection to map
				//or default to public
			}
		/*
		check connection to config.public.network.socket
		if no config.auth=0
		*/
/*
	  	drawScene();
		if (config.map.clean_textures_size!=0){
			glDeleteTextures(config.map.clean_textures_size,(unsigned int*)config.map.clean_textures);
			config.map.clean_textures_size=0;
		}
*/
	}
	
	config.map.enable=0;
	SDL_Delay(20);
/*
//	if (config.map.worker!=0){
		SDL_WaitThread(config.map.worker, 0);
		config.map.worker=0;
		SDL_WaitThread(config.map.connector, 0);
		config.map.connector=0;
//	}
*/
//	SDL_WaitThread(config.drawer, 0);
//	glFontDestroy(&font);
	cleanAll();
	realizeLights();
	networkExit();
	printf("exit\n");
//	SDL_Delay(100);
	SDL_DestroyWindow(config.window);
	SDL_Quit(); // завершаем работу SDL и выходим
	return 0;
}

void drawCube(float xrf, float yrf, float zrf){
	
	//glTranslatef(0.0f, 0.0f, -7.0f);	// Сдвинуть вглубь экрана
	
	glRotatef(xrf, 1.0f, 0.0f, 0.0f);	// Вращение куба по X, Y, Z
	glRotatef(yrf, 0.0f, 1.0f, 0.0f);	// Вращение куба по X, Y, Z
	glRotatef(zrf, 0.0f, 0.0f, 1.0f);	// Вращение куба по X, Y, Z
	
	glColor4f(0.0f, 1.0f, 0.0f,0.5f);		// Синяя сторона (Верхняя)
	
	glBegin(GL_QUADS);					// Рисуем куб

	glColor4f(0.0f, 1.0f, 0.0f,0.5f);		// Синяя сторона (Верхняя)
	glVertex3f( 1.0f, 1.0f, -1.0f);		// Верхний правый угол квадрата
	glVertex3f(-1.0f, 1.0f, -1.0f);		// Верхний левый
	glVertex3f(-1.0f, 1.0f,  1.0f);		// Нижний левый
	glVertex3f( 1.0f, 1.0f,  1.0f);		// Нижний правый
	
	glColor4f(1.0f, 0.5f, 0.0f,0.5f);		// Оранжевая сторона (Нижняя)
	glVertex3f( 1.0f, -1.0f,  1.0f);	// Верхний правый угол квадрата
	glVertex3f(-1.0f, -1.0f,  1.0f);	// Верхний левый
	glVertex3f(-1.0f, -1.0f, -1.0f);	// Нижний левый
	glVertex3f( 1.0f, -1.0f, -1.0f);	// Нижний правый
	
	glColor4f(1.0f, 0.0f, 0.0f,0.5f);		// Красная сторона (Передняя)
	glVertex3f( 1.0f,  1.0f, 1.0f);		// Верхний правый угол квадрата
	glVertex3f(-1.0f,  1.0f, 1.0f);		// Верхний левый
	glVertex3f(-1.0f, -1.0f, 1.0f);		// Нижний левый
	glVertex3f( 1.0f, -1.0f, 1.0f);		// Нижний правый

	glColor4f(1.0f,1.0f,0.0f,0.5f);			// Желтая сторона (Задняя)
	glVertex3f( 1.0f, -1.0f, -1.0f);	// Верхний правый угол квадрата
	glVertex3f(-1.0f, -1.0f, -1.0f);	// Верхний левый
	glVertex3f(-1.0f,  1.0f, -1.0f);	// Нижний левый
	glVertex3f( 1.0f,  1.0f, -1.0f);	// Нижний правый

	glColor4f(0.0f,0.0f,1.0f,0.5f);			// Синяя сторона (Левая)
	glVertex3f(-1.0f,  1.0f,  1.0f);	// Верхний правый угол квадрата
	glVertex3f(-1.0f,  1.0f, -1.0f);	// Верхний левый
	glVertex3f(-1.0f, -1.0f, -1.0f);	// Нижний левый
	glVertex3f(-1.0f, -1.0f,  1.0f);	// Нижний правый
	
	glColor4f(1.0f,0.0f,1.0f,0.5f);			// Фиолетовая сторона (Правая)
	glVertex3f( 1.0f,  1.0f, -1.0f);	// Верхний правый угол квадрата
	glVertex3f( 1.0f,  1.0f,  1.0f);	// Верхний левый
	glVertex3f( 1.0f, -1.0f,  1.0f);	// Нижний левый
	glVertex3f( 1.0f, -1.0f, -1.0f);	// Нижний правый

	glEnd();							// Закончили квадраты    

}