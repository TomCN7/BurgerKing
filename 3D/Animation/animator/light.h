#ifndef LIGHT_INCLUDED
#define LIGHT_INCLUDED
#include "glut.h"
#include "shape.h"

/* This is the abstract light class that all light sources must implement.
 * Each light has a color (the color of the light) and must provide ways for
 * reading/writing, abtaining diffuse and spectral contributions of the light
 * source at a point of intersection and determining if the the point of
 * intersection is in shadow with respect to the light source. */

class Light{
 public:
  Point3D color;

  virtual int read(FILE* fp)=0;
  virtual void write(FILE* fp=stdout)=0;

  virtual void draw(int lightNum)=0;
};
#endif /* RAY_LIGHT_INCLUDED */
  

