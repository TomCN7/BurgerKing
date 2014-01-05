#ifndef SPHERE_INCLUDED
#define SPHERE_INCLUDED
#include "geometry.h"
#include "shape.h"
#include "scene.h"

/* This class describes a sphere, defined by its center point and its radius. 
 */

class Sphere : public Shape {
 public:
  Point3D center;
  Flt radius;
  class Material *material;

  Sphere(Point3D center, Flt radius, Material* material);
  Sphere(FILE* fp,int* materialIndex);

  char* name(void);

  void write(int indent,FILE* fp=stdout);

  void draw(int complexity);
};

#endif /* SPHERE_INCLUDED */
  
