/*******************************************
 *
 * Jan 30, 2003: Modified by Renato Werneck
 *
 *******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "glut.h"
#include "sphere.h"

Sphere::Sphere(Point3D center, Flt radius, Material* material){
  this->center=center;
  this->radius=radius;
  this->material=material;
}
Sphere::Sphere(FILE* fp,int* index){
  if(fscanf(fp," %d %lg %lg %lg %lg",index,
	    &(center[0]),&(center[1]),&(center[2]), &radius) != 5){ 
    fprintf(stderr, "Failed to parse shape_sphere for Sphere\n"); 
    exit(EXIT_FAILURE);
  }
}
char* Sphere::name(void){return "sphere";}
void  Sphere::write(int indent,FILE* fp){
  int i;
  for(i=0;i<indent;i++){fprintf(fp," ");}
  fprintf(fp,"#shape_sphere %d\n",material->index);
  for(i=0;i<indent;i++){fprintf(fp," ");}
  fprintf(fp,"  %lg %lg %lg\n",center[0],center[1],center[2]);
  for(i=0;i<indent;i++){fprintf(fp," ");}
  fprintf(fp,"  %lg\n",radius);
}
void Sphere::draw(int complexity){
  GLUquadricObj* qobj=gluNewQuadric();
  glCallList(material->handle);
  gluQuadricDrawStyle(qobj,(GLenum)GLU_FILL);
  gluQuadricOrientation(qobj,(GLenum)GLU_OUTSIDE);
  gluQuadricNormals(qobj,(GLenum)GLU_SMOOTH);
  gluQuadricTexture(qobj,(GLenum)GL_TRUE);
  
  glPushMatrix();
  glTranslated(center[0],center[1],center[2]);
  glRotated(90.0,1.0,0.0,0.0);
  gluSphere(qobj,radius,complexity,complexity);
  glPopMatrix();
  
  gluDeleteQuadric(qobj);
}
