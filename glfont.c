//*********************************************************
//GLFONT.CPP -- glFont routines
//Copyright (c) 1998 Brad Fish
//See glFont.txt for terms of use
//November 10, 1998
//*********************************************************

//#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
//#include <GL/gl.h>
#include "headers.h"

//#include "glfont.h"


//*********************************************************
//Variables
//*********************************************************

//Current font
//GLFONT *glFont;

//*********************************************************
//Functions
//*********************************************************
int glFontCreate (GLFONT *Font, char *FileName)
{
	FILE *Input;
	char *TexBytes;
	int Num;
	char filepath[50];
	sprintf(filepath,"../locale/%s.glf",FileName);
	//Open font file
	if ((Input = fopen(filepath, "rb")) == NULL)
		return FALSE;

	//Read glFont structure
	fread(Font, sizeof(GLFONT), 1, Input);

	//Save texture number
	Font->Tex=getNewTexture();
	//glGenTextures (1, (unsigned int*)&Font->Tex);

	//Get number of characters
	Num = Font->IntEnd - Font->IntStart + 1;
	
	//Allocate memory for characters
	if ((Font->Char = (GLFONTCHAR *)malloc(
		sizeof(GLFONTCHAR) * Num)) == NULL)
		return FALSE;
	
	//Read glFont characters
	fread(Font->Char, sizeof(GLFONTCHAR), Num, Input);

	//Get texture size
	Num = Font->TexWidth * Font->TexHeight * 2;
	
	//Allocate memory for texture data
	if ((TexBytes = (char *)malloc(Num)) == NULL)
		return FALSE;
	
	//Read texture data
	fread(TexBytes, sizeof(char), Num, Input);

	//Set texture attributes
	glBindTexture(GL_TEXTURE_2D, Font->Tex);  
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
		GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
		GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
		GL_MODULATE);  
	
	//Create texture
	glTexImage2D(GL_TEXTURE_2D, 0, 2, Font->TexWidth,
		Font->TexHeight, 0, GL_LUMINANCE_ALPHA, 
		GL_UNSIGNED_BYTE, (void *)TexBytes);

	//Clean up
	free(TexBytes);
	fclose(Input);

	//Return pointer to new font
	return TRUE;
}
//*********************************************************
void glFontDestroy (GLFONT *Font)
{
	//Free character memory
	free(Font->Char);
}
//*********************************************************
/*
void glFontBegin (GLFONT *Font)
{
	//Save pointer to font structure
	if (Font->Char != NULL)
		glFont = Font;
	else
		glFont = NULL;
	
	//Bind to font texture
	glBindTexture(GL_TEXTURE_2D, Font->Tex);
}
*********************************************************
void glFontEnd (void)
{
	//Font no longer current
	glFont = NULL;
}
*/
//*********************************************************
#define size 0.606f //0.72f/*16*/ //0.66f /*17*/
#define  glFont_TexWidth  glFont->TexWidth*size
#define  glFont_TexHeight  glFont->TexHeight*size

void glFontTextOut (char2 *String, float x, float y,  float z)
{
	int i;
//	float height=0;
//	float _x=x;
	GLFONTCHAR *Char;
	GLFONT *glFont,*_glFont;
	float _width, _height;
	
	//Return if we don't have a valid glFont 
//	if (glFont == NULL)
//		return;
	glFont=localeFontGet(String[0]);
	if (glFont!=0){
		if (glFont->Tex==0)
			glFontCreate (glFont,localeFontPath(String[0]));
		glBindTexture(GL_TEXTURE_2D, glFont->Tex);
	}	
	//Begin rendering quads
	glBegin(GL_TRIANGLE_STRIP);
		
	//Loop through characters
	for (i = 0; String[i] !=0; i++)
	{
		//get font
		_glFont=localeFontGet(String[i]);
		if (_glFont==0)
			continue;
		//check if font need to change
		if (_glFont!=glFont){
//			printf("font change\n");
			glEnd();
			glFont=_glFont;
			if (glFont->Tex==0)
				glFontCreate (glFont,localeFontPath(String[i]));
			glBindTexture(GL_TEXTURE_2D, glFont->Tex);
			glBegin(GL_TRIANGLE_STRIP);
			
		}
//		printf("0x%x %d\n",String[i],glFont->IntStart);
		//Get pointer to glFont character
		Char = &glFont->Char[(int)String[i] -
			glFont->IntStart];
		//Get width and height
		_width = Char->dx * glFont_TexWidth;
		_height =  Char->dy * glFont_TexHeight;
		
		//Specify vertices and texture coordinates
		glTexCoord2f(Char->tx1, Char->ty1);
		glVertex3f(x, y, z);
		glTexCoord2f(Char->tx1, Char->ty2);
		glVertex3f(x, y - _height, z);
		glTexCoord2f(Char->tx2, Char->ty1);
		glVertex3f(x + _width, y, z);
		glTexCoord2f(Char->tx2, Char->ty2);
		glVertex3f(x + _width, y - _height, z);
		
//		if (height<_height)
//			height=_height;
		//Move to next character
		x += _width;
		
	}
	//Stop rendering quads
	glEnd();
	
	
}
//*********************************************************

float glFontWigth(char2 *String){
	int  i;
	float x=0;
	GLFONTCHAR *Char;
	GLFONT *glFont,*_glFont;
	//Return if we don't have a valid glFont 
//	if (glFont == NULL)
//		return 0;
	
	glFont=localeFontGet(String[0]);
	if (glFont->Tex==0)
		glFontCreate (glFont,localeFontPath(String[0]));
	
		
	//Loop through characters
	for (i = 0; String[i] !=0; i++)
	{
		//get font
		_glFont=localeFontGet(String[i]);
		if (_glFont==0)
			continue;
		//check if font need to change
		if (_glFont!=glFont){
			glFont=_glFont;
			if (glFont->Tex==0)
				glFontCreate (glFont,localeFontPath(String[i]));
		}
		
		//Get pointer to glFont character
		Char = &glFont->Char[(int)String[i] -
			glFont->IntStart];
			
		//Move to next character
		x += Char->dx * glFont_TexWidth;
	}

	return x;
}

float glFontHeight (char2 *String)
{
	int  i;
	float y=0;
	GLFONTCHAR *Char;
	GLFONT *glFont,*_glFont;
	//Return if we don't have a valid glFont 
//	if (glFont == NULL)
//		return 0;
	
	glFont=localeFontGet(String[0]);
	if (glFont->Tex==0)
		glFontCreate (glFont,localeFontPath(String[0]));
	

	//Loop through characters
	for (i = 0; String[i] !=0; i++)
	{
		//get font
		_glFont=localeFontGet(String[i]);
		if (_glFont==0)
			continue;
		//check if font need to change
		if (_glFont!=glFont){
			glFont=_glFont;
			if (glFont->Tex==0)
				glFontCreate (glFont,localeFontPath(String[i]));
		}
		
		//Get pointer to glFont character
		Char = &glFont->Char[(int)String[i] -
			glFont->IntStart];
	
		//Move to next character
		if (Char->dy>y*glFont_TexHeight)
			y=Char->dy*glFont_TexHeight;
	}

	return y;
}


//End of file



