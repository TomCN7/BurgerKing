#ifndef RAY_FILE_INSTANCE_INCLUDED
#define RAY_FILE_INSTANCE_INCLUDED
#include "geometry.h"
#include "shape.h"
#include "scene.h"

/* The RayFileInstance class allows a scene-graph to have a handle onto another
 * scene-graph read out from another .ray file. Since it is possible that
 * scene-graph from the .ray file might be included multiple times, this
 * class is essentially a container for a pointer to the scene-graph.
 * It is a subclass of Shape and implements the Shape methods by calling the
 * corresponding methods in the scene-graph pointed to. */

class RayFileInstance : public Shape {
	RayFile* rayFile;
	public:
	RayFileInstance(RayFile* rayFile);

	char* name(void);

	void write(int indent,FILE* fp=stdout);

	void draw(int complexity);
};

#endif /* RAY_FILE_INSTANCE_INCLUDED */

