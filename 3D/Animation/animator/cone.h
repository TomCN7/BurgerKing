#ifndef CONE_INCLUDED
#define CONE_INCLUDED
#include "geometry.h"
#include "shape.h"
#include "scene.h"

/* This class represents a cone with central axis parallel to the y-axis, and 
 * is defined by the center of the cone, the height from the tip to the base
 * and the base of the cone. */

class Cone : public Shape {
 public:
  Point3D center;
  Flt height,radius;
  class Material *material;


  Cone(Point3D center, Flt height, Flt radius, Material* material);
  Cone(FILE* fp,int* materialIndex);

  char* name(void);

  void write(int indent,FILE* fp=stdout);

  void draw(int complexity);
};

#endif /* CONE_INCLUDED */
