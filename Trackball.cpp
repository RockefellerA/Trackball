#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <GL/glut.h>
#include <iostream>

#define bool int
#define false 0
#define true 1
using namespace std;

int winWidth, winHeight;

float angle = 0.0, axis[3], trans[3];
bool trackingMouse = false;
bool redrawContinue = false;
bool trackballMove = false;

GLfloat vertices[][3]={
	{-1.0,-1.0,-1.0}, {1.0,-1.0,-1.0}, {1.0,1.0,-1.0}, {-1.0,1.0,-1.0},
	{-1.0,-1.0,1.0}, {1.0,-1.0,1.0}, {1.0,1.0,1.0}, {-1.0,1.0,1.0}
};
GLfloat colors[][3]={
	{0,0,0}, {1,0,0}, {1,1,0}, {0,1,0},
	{0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}
};

void polygon(int a, int b, int c, int d, int face)
{
  glBegin(GL_POLYGON);
    glColor3fv(colors[a]);
    glVertex3fv(vertices[a]);
    glVertex3fv(vertices[b]);
    glVertex3fv(vertices[c]);
    glVertex3fv(vertices[d]);
  glEnd();
}

void colorcube()
{
  polygon(1,0,3,2,0);
  polygon(3,7,6,2,1);
  polygon(7,3,0,4,2);
  polygon(2,6,5,1,3);
  polygon(4,5,6,7,4);
  polygon(5,4,0,1,5);
}

float lastPos[3] = {0.0F, 0.0F, 0.0F};
int curx, cury;
int startX, startY;

void trackball_ptov(int x, int y, int width, int height, float v[3])
{
  float d, a;
  
  v[0] = (2.0F*x - width) / width;
  v[1] = (height - 2.0F*y) / height;
  d = (float) sqrt(v[0]*v[0] + v[1]*v[1]);
  v[2] = (float) cos((M_PI/2.0F) * ((d < 1.0F) ? d : 1.0F));
  a = 1.0F / (float) sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
  v[0] *= a;
  v[1] *= a;
  v[2] *= a;
}

void mouseMotion(int x, int y)
{
  float curPos[3], dx, dy, dz;
  
  trackball_ptov(x, y, winWidth, winHeight, curPos);
if (trackingMouse)
{
  dx = curPos[0] - lastPos[0];
  dy = curPos[1] - lastPos[1];
  dz = curPos[2] - lastPos[2];
  
  if (dx || dy || dz) {
    angle = 90.0F * sqrt(dx*dx + dy*dy + dz*dz);
    
    axis[0] = lastPos[1]*curPos[2] - lastPos[2]*curPos[1];
    axis[1] = lastPos[2]*curPos[0] - lastPos[0]*curPos[2];
    axis[2] = lastPos[0]*curPos[1] - lastPos[1]*curPos[0];
    
    lastPos[0] = curPos[0];
    lastPos[1] = curPos[1];
    lastPos[2] = curPos[2];
  }
}
glutPostRedisplay();
}

void startMotion(int x, int y)
{
  trackingMouse = true;
  redrawContinue = false;
  startX = x; startY = y;
  curx = x; cury = y;
  trackball_ptov(x, y, winWidth, winHeight, lastPos);
  trackballMove = true;
}
void stopMotion(int x, int y)
{
  trackingMouse = false;
  
  if (startX != x || startY != y) {
    redrawContinue = true;
  }
  else {
    angle = 0.0F;
    redrawContinue = false;
    trackballMove = false;
  }
}
GLuint LoadTextureRAW( const char * filename, int wrap )
{
    GLuint texture;
    int width, height;
    GLubyte * data;
    FILE * file;

    // open texture data
    file = fopen( filename, "rb" );
    if ( file == NULL ){
	cout<<"Failed to open file";
	return 0;
	}

    // allocate buffer
    width = 512;
    height = 512;
    data = new GLubyte[width * height * 3];

    // read texture data
    fread( data, width * height * 3, 1, file );
    fclose( file );

    // allocate a texture name
    glGenTextures( 1, &texture );

    // select our current texture
    glBindTexture( GL_TEXTURE_2D, texture );

    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                     wrap ? GL_REPEAT : GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                     wrap ? GL_REPEAT : GL_CLAMP );

    // build our texture mipmaps
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,
                       GL_RGB, GL_UNSIGNED_BYTE, data );
	

    // free buffer
    free( data );

    return texture;
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	/* View transform */
	
	if (trackballMove)
		glRotatef(angle, axis[0], axis[1], axis[2]);
	glEnable( GL_TEXTURE_2D );
	GLuint text1 = LoadTextureRAW("textures/die1.png",true); //Opposite sides create a sum of 7.
	GLuint text2 = LoadTextureRAW("textures/die2.png",true);
	GLuint text3 = LoadTextureRAW("textures/die3.png",true);
	GLuint text4 = LoadTextureRAW("textures/die4.png",true);
	GLuint text5 = LoadTextureRAW("textures/die5.png",true);
	GLuint text6 = LoadTextureRAW("textures/die6.png",true);
	glBindTexture(GL_TEXTURE_2D,text1);
	glBegin(GL_QUADS);
		// Front Face
		glNormal3f( 0.0f, 0.0f, 0.5f);					
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D,text6);
	glBegin(GL_QUADS);
		// Back Face
		glNormal3f( 0.0f, 0.0f,-0.5f);					
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D,text3);
	glBegin(GL_QUADS);
		// Top Face
		glNormal3f( 0.0f, 0.5f, 0.0f);					
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D,text4);
	glBegin(GL_QUADS);
		// Bottom Face
		glNormal3f( 0.0f,-0.5f, 0.0f);					
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D,text5);
	glBegin(GL_QUADS);
		// Right Face
		glNormal3f( 0.5f, 0.0f, 0.0f);					
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D,text2);
	glBegin(GL_QUADS);
		// Left Face
		glNormal3f(-0.5f, 0.0f, 0.0f);					
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();
	glFlush();
	glDeleteTextures(1,&text1);
	glDeleteTextures(1,&text2);
	glDeleteTextures(1,&text3);
	glDeleteTextures(1,&text4);
	glDeleteTextures(1,&text5);
	glDeleteTextures(1,&text6);
	
	//colorcube();
	glutSwapBuffers();
}

void mouseButton(int button, int state, int x, int y)
{
  if (button == GLUT_RIGHT_BUTTON) exit(0);
  if (button == GLUT_LEFT_BUTTON) switch(state)
  {
    case GLUT_DOWN:
    y = winHeight-y;
    startMotion( x,y);
    break;
    case GLUT_UP:
    stopMotion( x,y);
    break;
  }
}

void myReshape(int w, int h)
{
  glViewport(0,0,w,h);
  winWidth = w;
  winHeight = h;
}

void spinCube()
{
  if (redrawContinue) glutPostRedisplay();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500,500);
  glutCreateWindow("Dice!");
  glutReshapeFunc(myReshape);
  glutDisplayFunc(display);
  glutIdleFunc(spinCube);
  glutMouseFunc(mouseButton);
  glutMotionFunc(mouseMotion);
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
  glMatrixMode(GL_MODELVIEW);
  glutMainLoop();
}
