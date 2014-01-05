#ifndef SPOT_LIGHT_INCLUDED
#define SPOT_LIGHT_INCLUDED
#include "light.h"

/* This class describes a spot-light light-source. This light has a fixed 
 * position in space and a fixed direction. The light attenuates both as a
 * function of the distance from the light-source and the angle between the
 * line from the light to the point and the light direction. The cutOffAngle
 * specifies the angle beyond which the light has no contribution. */

class SpotLight : public Light{
 public:
  Point3D location;
  Point3D direction;

  Flt constAtten;
  Flt linearAtten;
  Flt quadAtten;

  Flt cutOffAngle;
  Flt dropOffRate;

  int read(FILE* fp);
  void write(FILE* fp=stdout);
  
  void draw(int lightNum);
};

#endif /* SPOT_LIGHT_INCLUDED */
  

