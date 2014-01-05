#include <assert.h>
#include "pMatrix.h"

Parameter::Parameter(void){value=0.0;}

void ParametrizedMatrix::setType(int t){
	assert(0<=t && t<TRANSFORM_COUNT);
	type=t;
}
int ParametrizedMatrix::getType(void){return type;}
float ParametrizedMatrix::defaultParameterValue(void){
	switch(type){
	case XTRANSLATE:
	case YTRANSLATE:
	case ZTRANSLATE:
	case XROTATE:
	case YROTATE:
	case ZROTATE:
		return 0.0;
	case XSCALE:
	case YSCALE:
	case ZSCALE:
		return 1.0;
	default:
		return 0.0;
	}
}
Matrix ParametrizedMatrix::getMatrix(float t){
	Matrix m=IdentityMatrix();
	switch(type){
	case XTRANSLATE:
		m(3,0)=t;
		break;
	case YTRANSLATE:
		m(3,1)=t;
		break;
	case ZTRANSLATE:
		m(3,2)=t;
		break;
	case XROTATE:
		m(1,1)=m(2,2)=cos(t);
		m(1,2)=-(m(2,1)=-sin(t));
		break;
	case YROTATE:
		m(0,0)=m(2,2)=cos(t);
		m(0,2)=-(m(2,0)=sin(t));
		break;
	case ZROTATE:
		m(0,0)=m(1,1)=cos(t);
		m(0,1)=-(m(1,0)=-sin(t));
		break;
	case XSCALE:
		m(0,0)=t;
		break;
	case YSCALE:
		m(1,1)=t;
		break;
	case ZSCALE:
		m(2,2)=t;
		break;
	}
	return m;
}
