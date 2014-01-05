#ifndef DIRECTIONAL_LIGHT_INCLUDED
#define DIRECTIONAL_LIGHT_INCLUDED
#include "light.h"
/* This class represents a directional light source. The light has no position
 * in space and hence has no attenuation properties. The direction specifies 
 * the direction in which the light is going (not the direction to the light).
 */

class DirectionalLight : public Light{
 public:
  Point3D direction;

  int read(FILE* fp);
  void write(FILE* fp=stdout);

  void draw(int lightNum);
};

#endif /* DIRECTIONAL_LIGHT_INCLUDED */
  

