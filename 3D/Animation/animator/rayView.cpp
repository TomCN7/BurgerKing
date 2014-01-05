/*******************************************
 *
 * Jan 30, 2003: Modified by Renato Werneck
 * 
 *******************************************/

#include "glut.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/timeb.h>
#ifndef WIN32
#include <sys/time.h>
#endif
#include "geometry.h"
#include "bmp.h"

#include "rayView.h"
#include "scene.h"

#define KEY_SPACE  ' '
#define KEY_ESCAPE  '\033'
#define KEY_a 'a'
#define KEY_z 'z'
#define KEY_A 'A'
#define KEY_Z 'Z'
#define KEY_p 'p'
#define KEY_P 'P'
#define KEY_W 'W'
#define KEY_w 'w'
#define KEY_S 'S'
#define KEY_s 's'


#define KEY_UPARROW  101
#define KEY_DOWNARROW  103
#define KEY_LEFTARROW  100
#define KEY_RIGHTARROW  102
#define KEY_PGUP  104
#define KEY_PGDN  105

#define MOVE_DIST 1.0
#define ROT_INC 180.0
#define FAR_Z_ADJUST 1.0;
#define MOVE_SCALE 0.05
#define ROTATE_SCALE 1.0

#define RAD_TO_DEG (180.0/3.1415926535)

/////////////////////////////////////////////////////////////////

int window_width;
int window_height;

int complexity;
int fileCount=0;
int snapshotCount=0;

GLdouble far_z = 1900.0;
GLdouble near_z = 0.1;

GLuint listLight;                      
GLuint listMaterialBase;							

int lastX, lastY;
int leftDown = 0;
int midDown = 0;
int lastMaterial = -1;
Scene rvScene;
Camera* camera;
Point3D sceneCenter;
double initial_time;

/////////////////////////////////////////////////////////////////
double GetTime(void){
#ifdef WIN32
	struct _timeb t;
	_ftime(&t);
	return t.time+t.millitm/1000.0;
#else
	struct timeval t;
	gettimeofday(&t,NULL);
	return t.tv_sec+(double)t.tv_usec/1000000;
#endif
}

/////////////////////////////////////////////////////////////////

void Init(void);
void Keyboard(unsigned char,int,int);
void SpecialKeys(int,int,int);
void Motion(int,int);
void DepressedMotion(int,int);
void IdleFunction(void);
void Mouse(int,int,int,int);
void MainMenu(int);
void ResetViewMatrix(void);
void Draw(void);
void GetMatrix(Matrix&);
void AdjustFarZ(void);
void SetupLights(void);

void DrawModeMenu(int);

void TakeSnapshot(char* fileName);


/*//////////////////////////////////////*/

void RayView(const char *fileName,int width,int height,int cplx){
	int foo = 1;
	char *foochar[] = {"foo!"};
	window_width=width;
	window_height=height;

	rvScene.read(fileName);
	camera=rvScene.camera;
	camera->aspectRatio = (Flt)width/(Flt)height;
	complexity = cplx;
	if (complexity<10) complexity=10;
	if (complexity>100) complexity=100;	
	glutInit( &foo, foochar );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(width,height);
	glutInitWindowPosition(0,0);       
	glutCreateWindow( ".ray Viewer" );
	rvScene.setHandles();
	sceneCenter=Point3D(0.0,0.0,0.0);;
	Init();
	glutMainLoop();	
}

void Init( void )
{
  int submenu1;
  /* Point3D lookAtPt; */
  
  glutDisplayFunc( Draw );     
  glutReshapeFunc( NULL );  
  glutKeyboardFunc( Keyboard );
  glutSpecialFunc( SpecialKeys );  
  glutMouseFunc( Mouse );          
  glutMotionFunc( DepressedMotion );
  glutPassiveMotionFunc( Motion );  
  glutVisibilityFunc( NULL );      
  glutIdleFunc( IdleFunction );            
  
  submenu1 = glutCreateMenu( DrawModeMenu );
  glutAddMenuEntry( " Filled ", GL_FILL );
  glutAddMenuEntry( " Outline ", GL_LINE );

  glutCreateMenu( MainMenu );
  glutAddSubMenu(" Polygon mode ", submenu1);
  glutAddMenuEntry(" Write to File ", KEY_W );
  glutAddMenuEntry(" Take Snapshot ", KEY_S );
  glutAddMenuEntry( " Camera? ", KEY_P );
  glutAddMenuEntry( " Quit ", KEY_ESCAPE );

  glutAttachMenu( GLUT_RIGHT_BUTTON );
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glClearDepth(far_z);
  glClearColor(rvScene.background[0],rvScene.background[1],
	       rvScene.background[2],0.f);

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  gluPerspective(2*camera->heightAngle*180.0/3.1415,camera->aspectRatio,
	  near_z,far_z);
  ResetViewMatrix();
  
  SetupLights();
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  initial_time=GetTime();
}
void IdleFunction(void){
	rvScene.setParameters(GetTime()-initial_time);
	glutPostRedisplay();
}

/////////////////////////////////////////////////////////////////

void ResetViewMatrix(void){

  Point3D lookAtPt;
  
  glMatrixMode( GL_MODELVIEW );        
  glLoadIdentity();
  lookAtPt=camera->position+camera->direction;
  gluLookAt(camera->position[0],camera->position[1],camera->position[2],
	  lookAtPt[0],lookAtPt[1],lookAtPt[2],
	  camera->up[0],camera->up[1],camera->up[2]);
}


/////////////////////////////////////////////////////////////////

void Keyboard( unsigned char c, int x, int y )
{
  switch( c ){
  case KEY_ESCAPE:
    exit( 0 );
    break;
    
  case KEY_SPACE:
    
    break;
    
  case KEY_p:
  case KEY_P:
    fprintf(stderr,"\nPos: (%g,%g,%g)\n Dir: (%g,%g,%g)\n Up: (%g,%g,%g)\n",
	    camera->position[0],camera->position[1],camera->position[2],
	    camera->direction[0],camera->direction[1],camera->direction[2],
	    camera->up[0],camera->up[1],camera->up[2]);
    break;
    
  case KEY_A:
  case KEY_a:
    far_z += FAR_Z_ADJUST;
    AdjustFarZ();
    break;
    
  case KEY_Z:
  case KEY_z:
    far_z -= FAR_Z_ADJUST;
    if (far_z < near_z) far_z = near_z*2;
    AdjustFarZ();
    break;
  }
}
////////////////////////////////////////////////////////////////////

void SpecialKeys( int key, int x, int y )
{

  /* Point3D lookAtPt; */
  
  
  switch( key ){
    
  case KEY_UPARROW:
    camera->position=(camera->direction*MOVE_DIST)+
      camera->position;
  break;
  
  case KEY_DOWNARROW:
    camera->position=((-camera->direction)*MOVE_DIST)+
      camera->position;
    break;
    
  case KEY_LEFTARROW:
    camera->position=((-camera->right)*MOVE_DIST)+
      camera->position;
    break;
    
  case KEY_RIGHTARROW:
    camera->position=(camera->right*MOVE_DIST)+
      camera->position;
    break;
    
  case KEY_PGUP:
    camera->position=(camera->up*MOVE_DIST)+camera->position;
    break;

  case KEY_PGDN:
    camera->position=((-camera->up)*MOVE_DIST)+camera->position;
    break;
  }

  ResetViewMatrix();
  
  glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////
void AdjustFarZ(void){
}
////////////////////////////////////////////////////////////////////

void DepressedMotion( int x, int y ){
  GLdouble dx, dy;
  Matrix matrix;
  GLint viewport[4];
	
  dx = x - lastX;
  dy = y - lastY;
  
  lastX = x;
  lastY = y;
  
  glGetIntegerv(GL_VIEWPORT,viewport);
  if(leftDown==1 && midDown==1){
    dx*=MOVE_SCALE;
    dy*=MOVE_SCALE;
    camera->position=camera->position+
      camera->up*(dy)+camera->right*(-dx);
  }
  else if (leftDown == 1){
    dy*=MOVE_SCALE;
    glLoadIdentity();
    camera->position=camera->position+
      camera->direction*(-dy);
  } 
  else if (midDown == 1) {
    dx*=ROTATE_SCALE;
    dy*=ROTATE_SCALE;
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(sceneCenter[0],sceneCenter[1],sceneCenter[2]);
    glRotated(-dx,camera->up[0],camera->up[1],
	      camera->up[2]);
    glRotated(-dy,camera->right[0],camera->right[1],
	      camera->right[2]);
    glTranslatef(-sceneCenter[0],-sceneCenter[1],-sceneCenter[2]);
    GetMatrix(matrix);
    camera->position=matrix.multPosition(camera->position);
    camera->up=matrix.multDirection(camera->up).unit();
    camera->right=matrix.multDirection(camera->right).unit();
    camera->direction=
      matrix.multDirection(camera->direction).unit();
    glPopMatrix();
  }
  ResetViewMatrix();
  
  glutPostRedisplay();
}


////////////////////////////////////////////////////////////////////
void GetMatrix(Matrix& mat){
  GLdouble matrix[16];
  int row, col;
  
  glGetDoublev(GL_MODELVIEW_MATRIX,matrix);
  
  for(row=0;row<4;row++){
    for(col=0;col<4;col++){
      mat(col,row)=matrix[row+col*4];
    }
  }
}
////////////////////////////////////////////////////////////////////
void Motion( int x, int y) {



}

////////////////////////////////////////////////////////////////////

void Mouse( int button, int state, int x, int y )
{
  switch( button ){

  case GLUT_LEFT_BUTTON:
    if( state == GLUT_DOWN ){
      leftDown = 1;
      lastX = x;
      lastY = y;
    }
    else if( state == GLUT_UP ){
      leftDown = 0;
    }
    break;
  case GLUT_MIDDLE_BUTTON:
    if( state == GLUT_DOWN ){
      midDown = 1;
      lastX = x;
      lastY = y;
    }
    else if( state == GLUT_UP ){
      midDown = 0;
    }
    break;
  }

  //We could put the right button here, but its already bound to the menu

}
////////////////////////////////////////////////////////////////////
//
// This callback is invoked whever and entry of the drawmode sub
//  menu is selected.  entry will be the integer which we attached
//  to the chosen menu entry in the Init() function
//
/////////////////////////////////////////////////////////////////

void DrawModeMenu( int entry )
{
  glPolygonMode( GL_FRONT, (GLenum) entry );
  glPolygonMode( GL_BACK, (GLenum) entry );
  glutPostRedisplay();

}

////////////////////////////////////////////////////////////////////

void MainMenu( int entry )
{
  char fileName[500];
  FILE* fp;
  switch( entry ){
    
  case KEY_ESCAPE:
    exit( 0 );
    break;
  case KEY_p:
  case KEY_P:
    fprintf(stderr,"\nPos: (%g,%g,%g)\n Dir: (%g,%g,%g)\n Up: (%g,%g,%g)\n",
	    camera->position[0],camera->position[1],camera->position[2],
	    camera->direction[0],camera->direction[1],camera->direction[2],
	    camera->up[0],camera->up[1],camera->up[2]);
    break;
  case KEY_w:
  case KEY_W:
    sprintf(fileName,"temp%d.ray",fileCount);
    fileCount++;
    fp=fopen(fileName,"w");
    assert(fp);
    rvScene.write(fp);
    fclose(fp);
    printf("Wrote to file: %s\n",fileName);
    break;
  case KEY_S:
  case KEY_s:
    sprintf(fileName,"snapshot%d.bmp",snapshotCount);
    snapshotCount++;
    TakeSnapshot(fileName);
    printf("Wrote to file: %s\n",fileName);
    break;
  }
}
void TakeSnapshot(char* fileName){
  GLubyte *pixels;
  Image img(window_width,window_height);;
  int i,j;
  FILE *fp;

  pixels=new GLubyte[window_width*window_height*3];
  assert(pixels);
  fp=fopen(fileName,"w");
  if(fp==NULL){
    printf("Couldn't open snapshot.bmp for writing\n");
    return;
  }
  if(pixels==NULL){
    printf("Couldn't allocate memory for picture\n");
    return;
  }
  glReadBuffer(GL_FRONT);
  glReadPixels(0,0,window_width,window_height,GL_RGB,GL_UNSIGNED_BYTE,pixels);
  for(i=0;i<window_height;i++){
    for(j=0;j<window_width;j++){
      img(j,i).r=pixels[0+j*3+(window_height-i-1)*(window_width)*3];
      img(j,i).g=pixels[1+j*3+(window_height-i-1)*(window_width)*3];
      img(j,i).b=pixels[2+j*3+(window_height-i-1)*(window_width)*3];
    }
  }
  BMPWriteImage(&img,fp);
  fclose(fp);
  delete pixels;
}
///////////////////////////////////////////////////////////////////

void SetupLights(void){
  int i;
  
  GLfloat global_ambient[4];
  global_ambient[0] = rvScene.ambient[0];
  global_ambient[1] = rvScene.ambient[1];
  global_ambient[2] = rvScene.ambient[2];		
  global_ambient[3] = 1.0;      
  
  listLight=glGenLists(1);
  
  glNewList(listLight,GL_COMPILE);
  
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,global_ambient);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
  
  for (i=0;((i<rvScene.lightNum)&&(i<8));i++){rvScene.lights[i]->draw(i);}
  
  glEnable(GL_LIGHTING);
  
  glEndList();
}

///////////////////////////////////////////////////////////////////


void Draw( void )
{

	GLfloat position[4];
	GLfloat color[]={0.5,0.5,0.5,1.0};
	GLfloat ambient[]={0.0,0.0,0.0,1.0};

	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );

	if(rvScene.lightNum>0){glCallList(listLight);}
	else{
		position[0]=camera->position[0];
		position[1]=camera->position[1];
		position[2]=camera->position[2];
		position[3]=1.0;
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
		glLightfv(GL_LIGHT0, GL_SPECULAR, color);  
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}
	glPushMatrix();
	rvScene.group->draw(complexity);
	glPopMatrix();
	glutSwapBuffers();                  
}

////////////////////////////////////////////////////////////////////

