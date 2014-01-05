#ifndef TRANSFORM_INCLUDED 
#define TRANSFORM_INCLUDED
#include "geometry.h"

#define PARAMETER_NAME_SIZE 200

/* This class represents a parameter, it consists of a name and a floating
 * point value */
class Parameter{
public:
	float value;
	char name[PARAMETER_NAME_SIZE];

	Parameter(void);
};

enum{
	XTRANSLATE,
	YTRANSLATE,
	ZTRANSLATE,
	XROTATE,
	YROTATE,
	ZROTATE,
	XSCALE,
	YSCALE,
	ZSCALE,
	TRANSFORM_COUNT
};

class ParametrizedMatrix{
	int type;
public:
	void setType(int type);
	int  getType(void);
	float defaultParameterValue(void);
	Matrix getMatrix(float);
};
#endif /*TRANSFORM_INCLUDED */
