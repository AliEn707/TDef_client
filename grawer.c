#include "main.h"



void drawNode(){
	
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f (0.0f, 0.0f);
		glVertex2f(-0.5,-0.5);
		glTexCoord2f (1.0f, 0.0f);
		glVertex2f(0.5,-0.5);
		glTexCoord2f (0.0f, 1.0f);
		glVertex2f(-0.5,0.5);
		glTexCoord2f (1.0f, 1.0f);
		glVertex2f(0.5,0.5);
	glEnd();
}