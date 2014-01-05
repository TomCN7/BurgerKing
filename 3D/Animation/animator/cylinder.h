#ifndef CYLINDER_INCLUDED
#define CYLINDER_INCLUDED
#include "geometry.h"
#include "shape.h"
#include "scene.h"

/* This class represents a cylinder with central axis parallel to the y-axis, 
 * and is defined by the center of the cylinder, the height from the top cap
 * to the bottom cap, and the radius of the cylinder. */


class Cylinder : public Shape {
 public:
  Point3D center;
  Flt height,radius;
  class Material *material;

  Cylinder(Point3D center, Flt height, Flt radius, Material* material);
  Cylinder(FILE* fp,int* materialIndex);

  char* name(void);

  void write(int indent,FILE* fp=stdout);

  void draw(int complexity);
};

#endif /* CYLINDER_INCLUDED */
