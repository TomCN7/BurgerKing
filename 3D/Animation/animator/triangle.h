#ifndef TRIANGLE_INCLUDED
#define TRIANGLE_INCLUDED
#include "geometry.h"
#include "shape.h"
#include "scene.h"

/* This class represents a triangle and is specified by three pointers to
 * the three vertices that define it. */

class Triangle : public Shape {
 public:
  class Vertex* v[3];
  class Material *material;

  Triangle(Vertex* v1,Vertex* v2,Vertex* v3, Material* material);
  Triangle(FILE* fp,int* materialIndex,Vertex* vList,int vSize);

  char* name(void);

  void write(int indent,FILE* fp=stdout);

  void draw(int complexity);
};
#endif /* TRIANGLE_INCLUDED */
  

