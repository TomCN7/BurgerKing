/*******************************************
 * 
 * Jan 30, 2003: Modified by Renato Werneck
 *
 *******************************************/

#include <stdio.h>
#include <string.h>
#include "geometry.h"
#include "directionalLight.h"
#include "math.h"

int DirectionalLight::read(FILE* fp){
  if (fscanf(fp," %lg %lg %lg %lg %lg %lg",
	     &(color[0]),&(color[1]),&(color[2]),
	     &(direction[0]),&(direction[1]),&(direction[2])
	     ) != 6){return 0;}
  direction=direction.unit();
  return 1;
}
void DirectionalLight::write(FILE* fp){
  fprintf(fp,"#light_dir\n");
  fprintf(fp,"  %lg %lg %lg\n",color[0],color[1],color[2]);
  fprintf(fp,"  %lg %lg %lg\n",direction[0],direction[1],direction[2]);
}
void DirectionalLight::draw(int index){
  GLfloat l_position[4] ={0.0, 0.0, 0.0, 1.0};
  GLfloat l_ambient[4] = {0.0, 0.0, 0.0, 1.0};
  GLfloat l_diff_spec[4] = {0.0, 0.0, 0.0, 1.0};

  l_position[0]=-direction[0];
  l_position[1]=-direction[1];
  l_position[2]=-direction[2];			
  l_position[3]=0.0;
  l_diff_spec[0]=color[0];
  l_diff_spec[1]=color[1];
  l_diff_spec[2]=color[2];			
  glLightfv((GLenum)(GL_LIGHT0+index), GL_AMBIENT, l_ambient);
  glLightfv((GLenum)(GL_LIGHT0+index), GL_DIFFUSE, l_diff_spec);
  glLightfv((GLenum)(GL_LIGHT0+index), GL_SPECULAR, l_diff_spec);
  glLightfv((GLenum)(GL_LIGHT0+index), GL_POSITION, l_position);  
  glEnable((GLenum)(GL_LIGHT0+index));
}
