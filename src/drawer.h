int setTexture(texture * t);

void drawCursor();

//void drawNode(gnode* n);

void drawObject(object * o);

int drawMenu(menu * root);

void drawScene();

void globalTransform();

void backTransform();

//player info
#define ICON_SHIFT_X 5
#define ICON_SHIFT_Y 5
#define ICON_POS_X (SCREEN_OFFSET+ICON_SHIFT_X)
#define ICON_POS_Y (config.options.window.height-SCREEN_OFFSET-ICON_SHIFT_Y-ICON_SIZE)
#define ICON_SIZE 80
#define INFO_WIDTH (458-ICON_SIZE)
#define INFO_HEIGHT (ICON_SHIFT_Y+ICON_SIZE)
#define H_ICON_POS_X (ICON_POS_X+INFO_WIDTH)
#define H_ICON_POS_Y (ICON_POS_Y)
#define H_ICON_SIZE 80



#define Color4f(red,green,blue,alpha) glColor4f((red)*config.options.brightness*config.options.color.r,(green)*config.options.brightness*config.options.color.g,(blue)*config.options.brightness*config.options.color.b,alpha)

/*
//original
#define glRInit() 
#define Begin(x)  glBegin(x)
#define Vertex2f(x,y) glVertex2f((x),(y))
#define Vertex3f(x,y,z) glVertex3f((x),(y),(z))
#define TexCoord2f(x,y)  glTexCoord2f((x),(y))
#define End()  glEnd()
*/

//some funny functions
#define glRInit() glEnableClientState(GL_VERTEX_ARRAY);glTexCoordPointer(2, GL_FLOAT, 0, drawer.tex_coord);glEnableClientState(GL_TEXTURE_COORD_ARRAY)
#define Begin(x)  drawer.$vertex=0;drawer.items=0;drawer.draw_mode=x;drawer.$tex_coord=0
#define Vertex2f(x,y) drawer.shift=2;drawer.vertex[drawer.$vertex++]=(x);drawer.vertex[drawer.$vertex++]=(y)
#define Vertex3f(x,y,z) drawer.shift=3;drawer.vertex[drawer.$vertex++]=(x);drawer.vertex[drawer.$vertex++]=(y);drawer.vertex[drawer.$vertex++]=(z)
#define TexCoord2f(x,y)  drawer.tex_coord[drawer.$tex_coord++]=(x);drawer.tex_coord[drawer.$tex_coord++]=(y);drawer.items++
#define End()  if (drawer._shift!=drawer.shift){glVertexPointer(drawer.shift, GL_FLOAT, 0, drawer.vertex);drawer._shift=drawer.shift;}glDrawArrays(drawer.draw_mode,0,drawer.items)
