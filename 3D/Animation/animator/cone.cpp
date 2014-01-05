/********************************************
 *
 * Jan 30, 2003 - Modified by Renato Werneck
 *
 ********************************************/

#include <stdio.h>
#include <stdlib.h>
#include "glut.h"
#include "cone.h"

Cone::Cone(Point3D center, Flt height, Flt radius, Material* material){
  this->center=center;
  this->height=height;
  this->radius=radius;
  this->material=material;
}
Cone::Cone(FILE* fp,int* index){
  if(fscanf(fp," %d %lg %lg %lg %lg %lg",index,
	    &(center[0]),&(center[1]),&(center[2]),
	    &height,&radius) != 6){
    fprintf(stderr, "Failed to parse shape_cone for Cone\n"); 
    exit(EXIT_FAILURE);
  }
}
char* Cone::name(void){return "cone";}
void  Cone::write(int indent,FILE* fp){
  int i;

  for(i=0;i<indent;i++){fprintf(fp," ");}

  fprintf(fp,"#shape_cone %d\n",material->index);

  for(i=0;i<indent;i++){fprintf(fp," ");}

  fprintf(fp,"  %lg %lg %lg\n",center[0],center[1],center[2]);

  for(i=0;i<indent;i++){fprintf(fp," ");}

  fprintf(fp,"  %lg %lg\n",radius,height);
}


void Cone::draw(int complexity){
  GLUquadricObj* qobj=gluNewQuadric();

  glCallList(material->handle);
  gluQuadricDrawStyle(qobj, (GLenum)GLU_FILL);
  gluQuadricOrientation(qobj, (GLenum)GLU_OUTSIDE);
  gluQuadricNormals(qobj, (GLenum)GLU_SMOOTH);
  gluQuadricTexture(qobj,GL_TRUE);
  
  glPushMatrix();
  glTranslated(center[0],center[1]+height/2.0,center[2]);
  glRotated(90.0,1.0,0,0);
  gluCylinder( qobj,0.0,radius,height,complexity,complexity);
  glPushMatrix();
  glTranslated(0.0,0.0,height);
  gluDisk(qobj,0.0,radius,complexity,complexity);
  glPopMatrix();
  glPopMatrix();
  
  gluDeleteQuadric(qobj);
}
