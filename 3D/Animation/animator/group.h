#ifndef GROUP_INCLUDED
#define GROUP_INCLUDED
#include "geometry.h"
#include "shape.h"
#include "pMatrix.h"

/* The ShapeListElement class is used to store a list of shapes. It consists
 * of a pointer to the shape and a pointer to the next element of the list. */
class ShapeListElement {
	public:
		ShapeListElement* next;
		Shape* shape;

		ShapeListElement(class Shape* shape);
		~ShapeListElement(void);
		void addShape(class Shape* newShape);
};

/* This Group class represents a node in the scene graph. It consists of a 
 * list of shapes and a transformation that is to be applied to them. This
 * class is itself a subclass of Shape as it is ray-tracable. (A group is
 * ray-traced by ray-tracing each of its transformed children and obtaining
 * the closest point of intersection. Alternatively, and more simply, the
 * ray can be transformed appropriately and then one doesn't have to worry
 * about applying transformations to the child Shapes. */
class Group : public Shape{
	int sNum;
public:
	ShapeListElement *shapeList;

	Group(void);
	virtual ~Group(void);

	void addShape(Shape* shape);
	int  shapeNum(void);
	void draw(int complexity);
	void write(int indent,FILE* fp=stdout);

	virtual void writeBegin(int indent,FILE* fp=stdout)=0;
	virtual void writeEnd(int indent, FILE* fp=stdout)=0;
	virtual Matrix getMatrix(void)=0;
};

class StaticGroup : public Group{
public:
	Matrix localTransform;

	StaticGroup(FILE* fp);
	StaticGroup(Matrix localTransform);

	char* name(void);
	Matrix getMatrix(void);
	void writeBegin(int indent,FILE* fp=stdout);
	void writeEnd(int indent,FILE* fp=stdout);	
};

class ParametrizedGroup : public Group{
public:
	Parameter* parameter;
	ParametrizedMatrix localTransform;
	ParametrizedGroup(FILE* fp,char* parameterName);

	char* name(void);
	Matrix getMatrix(void);
	void writeBegin(int indent,FILE* fp=stdout);
	void writeEnd(int indent,FILE* fp=stdout);
};
#endif /* GROUP_INCLUDED */
