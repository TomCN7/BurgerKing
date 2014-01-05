#include <stdio.h>
#include <stdlib.h>
#include "glut.h"
#include "box.h"

Box::Box(Point3D center, Point3D length,Material* material){
  this->center=center;
  this->length=length;
  this->material=material;
}
Box::Box(FILE* fp,int* index){
  if(fscanf(fp," %d %lg %lg %lg %lg %lg %lg",index,
	    &(center[0]),&(center[1]),&(center[2]),
	    &(length[0]),&(length[1]),&(length[2])) != 7){
    fprintf(stderr, "Failed to parse shape_box for Box\n"); 
    exit(EXIT_FAILURE);
  }
}

char* Box::name(void){return "box";}
void  Box::write(int indent,FILE* fp){
  int i;

  for(i=0;i<indent;i++){fprintf(fp," ");}

  fprintf(fp,"#shape_box %d\n",material->index);

  for(i=0;i<indent;i++){fprintf(fp," ");}

  fprintf(fp,"  %lg %lg %lg\n",center[0],center[1],center[2]);

  for(i=0;i<indent;i++){fprintf(fp," ");}

  fprintf(fp,"  %lg %lg %lg\n",length[0],length[1],length[2]);
}

void Box::draw(int complexity){
  GLfloat norms[6][3]={{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}};
  GLfloat texs[4][2]={{0,0},{1,0},{1,1},{0,1}};
  GLdouble verts[8][3];

  verts[0][0]=0.;
  verts[0][1]=length[1];
  verts[0][2]=0.;
  verts[1][0]=length[0];
  verts[1][1]=length[1];
  verts[1][2]=0.;
  verts[2][0]=length[0];
  verts[2][1]=0.;
  verts[2][2]=0.;
  verts[3][0]=0.;
  verts[3][1]=0.;
  verts[3][2]=0.;
  verts[4][0]=length[0];
  verts[4][1]=length[1];
  verts[4][2]=-length[2];
  verts[5][0]=0.;
  verts[5][1]=length[1];
  verts[5][2]=-length[2];
  verts[6][0]=0.;
  verts[6][1]=0.;
  verts[6][2]=-length[2];
  verts[7][0]=length[0];
  verts[7][1]=0.;
  verts[7][2]=-length[2];

  glCallList(material->handle);
  glPushMatrix();
  glTranslated(center[0]-length[0]/2.0,center[1]-length[1]/2.0,
	       center[2]+length[2]/2.0);
  
  glBegin(GL_QUADS);
  glTexCoord2fv(&texs[3][0]);
  glNormal3fv(&norms[4][0]);
  glVertex3dv(verts[3]);

  glTexCoord2fv(&texs[2][0]);
  glNormal3fv(&norms[4][0]);
  glVertex3dv(verts[2]);
  
  glTexCoord2fv(&texs[1][0]);
  glNormal3fv(&norms[4][0]);
  glVertex3dv(verts[1]);
  
  glTexCoord2fv(&texs[0][0]);
  glNormal3fv(&norms[4][0]);
  glVertex3dv(verts[0]);
  
  glTexCoord2fv(&texs[3][0]);
  glNormal3fv(&norms[1][0]);
  glVertex3dv(&verts[2][0]);
  
  glTexCoord2fv(&texs[2][0]);
  glNormal3fv(&norms[1][0]);
  glVertex3dv(&verts[7][0]);
  
  glTexCoord2fv(&texs[1][0]);
  glNormal3fv(&norms[1][0]);
  glVertex3dv(&verts[4][0]);
  
  glTexCoord2fv(&texs[0][0]);
  glNormal3fv(&norms[1][0]);
  glVertex3dv(&verts[1][0]);
			
  glTexCoord2fv(&texs[3][0]);
  glNormal3fv(&norms[5][0]);
  glVertex3dv(&verts[7][0]);
  
  glTexCoord2fv(&texs[2][0]);
  glNormal3fv(&norms[5][0]);
  glVertex3dv(&verts[6][0]);
  
  glTexCoord2fv(&texs[1][0]);
  glNormal3fv(&norms[5][0]);
  glVertex3dv(&verts[5][0]);
  
  glTexCoord2fv(&texs[0][0]);
  glNormal3fv(&norms[5][0]);
  glVertex3dv(&verts[4][0]);
  
  glTexCoord2fv(&texs[3][0]);
  glNormal3fv(&norms[3][0]);
  glVertex3dv(&verts[6][0]);
  
  glTexCoord2fv(&texs[2][0]);
  glNormal3fv(&norms[2][0]);
  glVertex3dv(&verts[3][0]);
  
  glTexCoord2fv(&texs[1][0]);
  glNormal3fv(&norms[1][0]);
  glVertex3dv(&verts[0][0]);
  
  glTexCoord2fv(&texs[0][0]);
  glNormal3fv(&norms[0][0]);
  glVertex3dv(&verts[5][0]);
  
  glTexCoord2fv(&texs[3][0]);
  glNormal3fv(&norms[2][0]);
  glVertex3dv(&verts[0][0]);
  
  glTexCoord2fv(&texs[2][0]);
  glNormal3fv(&norms[2][0]);
  glVertex3dv(&verts[1][0]);

  glTexCoord2fv(&texs[1][0]);
  glNormal3fv(&norms[2][0]);
  glVertex3dv(&verts[4][0]);
  
  glTexCoord2fv(&texs[0][0]);
  glNormal3fv(&norms[2][0]);
  glVertex3dv(&verts[5][0]);
  
  glTexCoord2fv(&texs[3][0]);
  glNormal3fv(&norms[3][0]);
  glVertex3dv(&verts[6][0]);
  
  glTexCoord2fv(&texs[2][0]);
  glNormal3fv(&norms[3][0]);
  glVertex3dv(&verts[7][0]);
  
  glTexCoord2fv(&texs[1][0]);
  glNormal3fv(&norms[3][0]);
  glVertex3dv(&verts[2][0]);
  
  glTexCoord2fv(&texs[0][0]);
  glNormal3fv(&norms[3][0]);
  glVertex3dv(&verts[3][0]);

  glEnd();
  
  glPopMatrix();
}

  
