#include <windows.h> 


#include "../main.h"
#include "../glfont.h"
#include "../tga.h"
#include "../drawer.h"
#include "../engine.h"
#include "../file.h"





void drawCube(float xrf, float yrf, float zrf);


void frameSync(unsigned int *time){
	if (*time==0){
		*time=timeGetTime();
		return;
	}
		unsigned int t=config.time_per_frame-(timeGetTime()-*time);
	if (t<0){
		perror("Time");
		t=0;
	}
	Sleep(t<=config.time_per_frame?t:0);
	*time=timeGetTime();
}

void tickSync(unsigned int *time){
	Sleep(config.time_per_tick-(SDL_GetTicks()-*time));
	*time=SDL_GetTicks();
}


#undef main
int main(int argc, char *argv[]){   
	printf("Initializing.....");
	memset(&config,0,sizeof(config));
	config.window_width=800;
	config.window_height=600;
	config.time_per_frame=1000/60;
	graficsInit(); // инициализация
	printf("done\n");
	loadFiles();
	
//	FreeConsole();
	GLFONT font;
	glFontCreate (&font, "test.glf");
	
	
	
//	SetConsoleOutputCP(CP_UTF8);
//	printf("%ls\n",L"ПРИвет");
	float xrf = 0, yrf = 0, zrf = 0; // углы поворота
	Uint32 time=0;
	config.main_running=1;
	while(config.main_running){ 
		frameSync(&time);
		config.global_count++;
//		SDL_Delay(100);
		SDL_Event event;
		SDL_GetMouseState(&cursor.pos.x,&cursor.pos.y);
		checkMouseState();
		while( SDL_PollEvent(&event) ){
			processEvent(event);
		}
		processKeyboard();

	  	drawScene();
//....................................................
/*
		xrf -= 0.5; 
		yrf -= 0.5;
		zrf -= 0.5;
		
		
		glEnable(GL_DEPTH_TEST);
		
		glPushMatrix();
			glTranslatef(config.window_width/2,config.window_height/2,-20);
				
			glScalef(100,100,1);
			//glFontBegin(&font);
				
			//glFontEnd();
			glDisable (GL_TEXTURE_2D);
			drawCube(xrf, yrf, zrf); 
		 
		glPopMatrix();
*/		
		glEnable (GL_TEXTURE_2D);
		glTranslatef(0,config.window_height/2,0);
			glScalef(15,15,1);
			glColor4f(1.0f, 1.0f, 1.0f,1.0f);
				glFontTextOut(&font,"Hello world\n my friend",0,0,30);
				glScalef(10,10,1);
				glTranslatef(0,0,40);
//				drawNode();
//.........................................................
		glFlush();
		SDL_GL_SwapWindow(config.window);
	}
	
	
	glFontDestroy(&font);
	cleanAll();
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