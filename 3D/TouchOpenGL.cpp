#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <cstdio>
#include <cstdlib>

typedef unsigned char BYTE;

// This is just an example using basic glut functionality.
// // If you want specific Apple functionality, look up AGL

GLuint g_texture = 0;
float theta = .0f;

void init() // called before main loop to set up the program
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
//	glEnable(GL_DEPTH_TEST);
//	glShadeModel(GL_SMOOTH);
}

GLuint LoadTextureRAW(const char* filename, int wrap)
{
	int width, height;
	BYTE* data;
	FILE* file;

	// open texture data
	file = fopen( filename, "rb" );
	if ( file == NULL ) return 0;

	// allocate buffer
	width = 256;
	height = 256;
	data = (BYTE*)malloc( width * height * 3 );

	// read texture data
	fread( data, width * height * 3, 1, file );
	fclose( file );

	// allocate a texture name
	glGenTextures( 1, &g_texture );

	// select our current texture
	glBindTexture( GL_TEXTURE_2D, g_texture );

	// select modulate to mix texture with color for shading
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	// when texture area is small, bilinear filter the closest MIP map
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST );
	// when texture area is large, bilinear filter the first MIP map
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	// if wrap is true, the texture wraps over at the edges (repeat)
	//       ... false, the texture ends at the edges (clamp)
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP );

	// build our texture MIP maps
	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width,
			height, GL_RGB, GL_UNSIGNED_BYTE, data );

	// free buffer
	free( data );

	return g_texture;
}

void FreeTexture( GLuint texture )
{
	glDeleteTextures( 1, &texture );
}

// Called at the start of the program, after a glutPostRedisplay() and during idle
// to display a frame
void display()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	// 清除屏幕和深度缓存
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	// 重置当前矩阵
//	glLoadIdentity();

//	glBegin(GL_TRIANGLES);
//	glVertex3f(0.0, 0.0, -10.0);
//	glVertex3f(1.0, 0.0, -10.0);
//	glVertex3f(0.0, 1.0, -10.0);
//	glEnd();

	// setup texture mapping
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, g_texture );

	glPushMatrix();
	glRotatef( theta, 0.0f, 0.0f, 1.0f );
	glBegin( GL_QUADS );
	glTexCoord2d(0.0,0.0); 
	glVertex2d(-1.0,-1.0);
	glTexCoord2d(1.0,0.0); 
	glVertex2d(+1.0,-1.0);
	glTexCoord2d(1.0,1.0); 
	glVertex2d(+1.0,+1.0);
	glTexCoord2d(0.0,1.0); 
	glVertex2d(-1.0,+1.0);
	glEnd();
	glPopMatrix();

	theta += 1.0f;

	glutSwapBuffers();
}

// Called every time a window is resized to resize the projection matrix
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.1, 0.1, -float(h)/(10.0*float(w)), float(h)/(10.0*float(w)), 0.5, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


int main(int argc, char **argv)
{

	glutInit(&argc, argv); // Initializes glut

	// Sets up a double buffer with RGBA components and a depth component
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

	// Sets the window size to 512*512 square pixels
	glutInitWindowSize(512, 512);

	// Sets the window position to the upper left
	glutInitWindowPosition(0, 0);

	// Creates a window using internal glut functionality
	glutCreateWindow("Hello!");

	// passes reshape and display functions to the OpenGL machine for callback
//	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);

	init();

	g_texture = LoadTextureRAW("texture.raw", true);
	if (!g_texture)
	{
		printf("texture load error!\n");
		return -1;
	}

	// Starts the program.
	glutMainLoop();

	FreeTexture(g_texture);
	return 0;
}
