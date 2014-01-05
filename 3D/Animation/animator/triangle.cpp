#include <stdio.h>
#include <stdlib.h>
#include "glut.h"
#include "triangle.h"

Triangle::Triangle(Vertex* v1,Vertex* v2,Vertex* v3, Material* material){
  v[0]=v1;
  v[1]=v2;
  v[2]=v3;
  this->material=material;
}
Triangle::Triangle(FILE* fp,int* index,Vertex* vList,int vSize){
  int v1,v2,v3;
  if(fscanf(fp," %d %d %d %d",index,&v1,&v2,&v3) != 4){
    fprintf(stderr, "Failed to parse shape_triangle for Triangle\n"); 
    exit(EXIT_FAILURE);
  }
  if(v1<0 || v1>=vSize || v2<0 || v2>=vSize || v3<0 || v3>=vSize){
    fprintf(stderr, "Vertex index not within bounds for Triangle\n");
    exit(EXIT_FAILURE);
  }
  if(v1==v2 || v1==v3 || v2==v3){
    fprintf(stderr,"Vertices must be distinct in Triangle (%d %d %d)\n",
	    v1,v2,v3);
    exit(EXIT_FAILURE);
  }
  v[0]=&(vList[v1]);
  v[1]=&(vList[v2]);
  v[2]=&(vList[v3]);
}
void  Triangle::write(int indent,FILE* fp){
  int i;
  for(i=0;i<indent;i++){fprintf(fp," ");}
  fprintf(fp,"#shape_triangle %d\n",material->index);
  for(i=0;i<indent;i++){fprintf(fp," ");}
  fprintf(fp,"  %d %d %d\n",v[0]->index,v[1]->index,v[2]->index);
}
char* Triangle::name(void){return "triangle";}
void Triangle::draw(int complexity){
  int i;
  GLdouble norms[3][3];
  GLdouble  texs[3][2];
  GLdouble verts[3][3];
  
  for (i=0;i<3;i++){
    verts[i][0]=v[i]->position[0];
    verts[i][1]=v[i]->position[1];
    verts[i][2]=v[i]->position[2];		
    norms[i][0]=v[i]->normal[0];
    norms[i][1]=v[i]->normal[1];
    norms[i][2]=v[i]->normal[2];
    texs[i][0]=v[i]->texCoordinate[0];
    texs[i][1]=v[i]->texCoordinate[1];
  }
	
  glCallList(material->handle);
  glBegin(GL_TRIANGLES);
  glTexCoord2dv(texs[0]);glNormal3dv(norms[0]);	glVertex3dv(verts[0]);
  glTexCoord2dv(texs[1]);glNormal3dv(norms[1]);	glVertex3dv(verts[1]);
  glTexCoord2dv(texs[2]);glNormal3dv(norms[2]);	glVertex3dv(verts[2]);
  glEnd();
}
