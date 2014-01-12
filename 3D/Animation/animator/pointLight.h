#ifndef POINT_LIGHT_INCLUDED
#define POINT_LIGHT_INCLUDED
#include "light.h"

/* The PointLight class describes a light source with a fixed position in
 * space whose light distribution is independent of the angle at which the
 * light leaves the source. The light does, however, attenuate quadratically
 * with distance and the attenuation factors are used to describe the
 * strength of attenuation. */

class PointLight : public Light{
	public:
		Point3D location;
		Flt constAtten;
		Flt linearAtten;
		Flt quadAtten;		

		int read(FILE* fp);
		void write(FILE* fp=stdout);

		void draw(int lightNum);
};

#endif /* POINT_LIGHT_INCLUDED */


