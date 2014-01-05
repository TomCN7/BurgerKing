/*******************************************
 * 
 * Jan 30, 2003: Modified by Renato Werneck
 *
 *******************************************/

#include <stdio.h>
#include <string.h>
#include "geometry.h"
#include "spotLight.h"
#include "math.h"

int SpotLight::read(FILE* fp){
  if (fscanf(fp," %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg", 
	     &(color[0]),&(color[1]),&(color[2]),
	     &(location[0]),&(location[1]),&(location[2]),
	     &(direction[0]),&(direction[1]),&(direction[2]),
	     &(constAtten),&(linearAtten),&(quadAtten),
	     &(cutOffAngle),&(dropOffRate)) != 14){return 0;}
  direction=direction.unit();
  return 1;
}
void SpotLight::write(FILE* fp){
  fprintf(fp,"#light_spot\n");
  fprintf(fp,"  %lg %lg %lg\n",color[0],color[1],color[2]);
  fprintf(fp,"  %lg %lg %lg\n",location[0],location[1],location[2]);
  fprintf(fp,"  %lg %lg %lg\n",direction[0],direction[1],direction[2]);
  fprintf(fp,"  %lg %lg %lg\n",constAtten,linearAtten,quadAtten);
  fprintf(fp,"  %lg %lg\n",cutOffAngle,dropOffRate);
}
void SpotLight::draw(int index){
  GLfloat l_position[4] ={0.0, 0.0, 0.0, 1.0};
  GLfloat l_ambient[4] = {0.0, 0.0, 0.0, 1.0};
  GLfloat l_diff_spec[4] = {0.0, 0.0, 0.0, 1.0};
  GLfloat l_spotdir[3] = {0.0, 0.0, 0.0};

  l_position[0]=location[0];
  l_position[1]=location[1];
  l_position[2]=location[2];			
  l_position[3]=1.0;
  l_diff_spec[0]=color[0];
  l_diff_spec[1]=color[1];
  l_diff_spec[2]=color[2];			
  l_spotdir[0]=direction[0];
  l_spotdir[1]=direction[1];
  l_spotdir[2]=direction[2];			

  glLightfv((GLenum)(GL_LIGHT0+index), GL_AMBIENT, l_ambient);
  glLightfv((GLenum)(GL_LIGHT0+index), GL_DIFFUSE, l_diff_spec);
  glLightfv((GLenum)(GL_LIGHT0+index), GL_SPECULAR, l_diff_spec);

  glLightfv((GLenum)(GL_LIGHT0+index), GL_POSITION, l_position);  
  glLightfv((GLenum)(GL_LIGHT0+index), GL_SPOT_DIRECTION, l_spotdir);
  
  glLightf((GLenum)(GL_LIGHT0+index), GL_CONSTANT_ATTENUATION,constAtten);
  glLightf((GLenum)(GL_LIGHT0+index), GL_LINEAR_ATTENUATION,linearAtten);
  glLightf((GLenum)(GL_LIGHT0+index), GL_QUADRATIC_ATTENUATION,quadAtten);
  glLightf((GLenum)(GL_LIGHT0+index), GL_SPOT_EXPONENT,128.0*dropOffRate);
  glLightf((GLenum)(GL_LIGHT0+index), GL_SPOT_CUTOFF,180.0/3.14159265358979323846*cutOffAngle);
  
  glEnable((GLenum)(GL_LIGHT0+index));
}
