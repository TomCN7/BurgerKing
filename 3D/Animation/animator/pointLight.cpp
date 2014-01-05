#include <stdio.h>
#include <string.h>
#include "glut.h"
#include "geometry.h"
#include "pointLight.h"
#include "math.h"

int PointLight::read(FILE* fp){
  if(fscanf(fp," %lg %lg %lg %lg %lg %lg %lg %lg %lg", 
	    &(color[0]),&(color[1]),&(color[2]),
	    &(location[0]),&(location[1]),&(location[2]),
	    &(constAtten),&(linearAtten),&(quadAtten)) != 9){return 0;}
  return 1;
}
void PointLight::write(FILE* fp){
  fprintf(fp,"#light_point\n");
  fprintf(fp,"  %lg %lg %lg\n",color[0],color[1],color[2]);
  fprintf(fp,"  %lg %lg %lg\n",location[0],location[1],location[2]);
  fprintf(fp,"  %lg %lg %lg\n",constAtten,linearAtten,quadAtten);
}
void PointLight::draw(int index){
  GLfloat l_position[4] ={0.0, 0.0, 0.0, 1.0};
  GLfloat l_ambient[4] = {0.0, 0.0, 0.0, 1.0};
  GLfloat l_diff_spec[4] = {0.0, 0.0, 0.0, 1.0};

  l_position[0]=location[0];
  l_position[1]=location[1];
  l_position[2]=location[2];			
  l_position[3]=1.0;
  l_diff_spec[0]=color[0];
  l_diff_spec[1]=color[1];
  l_diff_spec[2]=color[2];			
  glLightfv((GLenum)(GL_LIGHT0+index), GL_AMBIENT, l_ambient);
  glLightfv((GLenum)(GL_LIGHT0+index), GL_DIFFUSE, l_diff_spec);
  glLightfv((GLenum)(GL_LIGHT0+index), GL_SPECULAR, l_diff_spec);

  glLightfv((GLenum)(GL_LIGHT0+index), GL_POSITION, l_position);  
  
  glLightf((GLenum)(GL_LIGHT0+index), GL_CONSTANT_ATTENUATION,constAtten);
  glLightf((GLenum)(GL_LIGHT0+index), GL_LINEAR_ATTENUATION,linearAtten);
  glLightf((GLenum)(GL_LIGHT0+index), GL_QUADRATIC_ATTENUATION,quadAtten);
  
  glLightf((GLenum)(GL_LIGHT0+index), GL_SPOT_CUTOFF, 180.0);
  glEnable((GLenum)(GL_LIGHT0+index));
}
