#ifndef BOX_INCLUDED
#define BOX_INCLUDED
#include "geometry.h"
#include "shape.h"
#include "scene.h"

/* This class represents a box and is defined by its center and the length of
 * the sides. */

class Box : public Shape {
 public:
  Point3D center;
  Point3D length;
  class Material *material;


  Box(Point3D center, Point3D length,Material* material);
  Box(FILE* fp,int* materialIndex);

  char* name(void);

  void write(int indent,FILE* fp=stdout);

  void draw(int complexity);
};

#endif /* BOX_INCLUDED */
  
