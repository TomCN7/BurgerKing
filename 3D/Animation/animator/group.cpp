#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "glut.h"
#include "group.h"
#include "string.h"
#include "pMatrix.h"

/* These are functions associated to the ShapeListElement class */
ShapeListElement::ShapeListElement(Shape* s){
	next=NULL;
	shape=s;
}
ShapeListElement::~ShapeListElement(void){
	if(next!=NULL){delete next;}
	if(shape!=NULL){delete shape;}
}

void ShapeListElement::addShape(Shape* s){
	ShapeListElement* temp;
	temp=next;
	next=new ShapeListElement(s);
	assert(next);
	next->next=temp;
}

/* These are functions associated to the Group class */
Group::Group(void){
	shapeList=0;
	sNum=0;
}

void Group::addShape(Shape* s){
	if(shapeList==NULL){
		shapeList=new ShapeListElement(s);
		sNum=0;
	}
	else{shapeList->addShape(s);}
	sNum++;
}
int Group::shapeNum(void){return sNum;}
void Group::write(int indent,FILE* fp){
	ShapeListElement* temp;

	writeBegin(indent,fp);
	temp=shapeList;
	while(temp!=NULL){
		temp->shape->write(indent+2,fp);
		temp=temp->next;
	}
	writeEnd(indent,fp);
}
void Group::draw(int cplx){
	ShapeListElement* temp;
	GLdouble m[16];
	Matrix matrix=getMatrix();
	int i,j;

	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			m[i+j*4]=matrix(j,i);
		}
	}
	if(shapeNum()){glPushMatrix();}
	glMultMatrixd(m);
	temp=shapeList;

	while(temp!=NULL){
		temp->shape->draw(cplx);
		temp=temp->next;
	}
	if(shapeNum()){glPopMatrix();}
}
Group::~Group(void){if(shapeList!=NULL){delete shapeList;}}

char* StaticGroup::name(void){return "static group";}
StaticGroup::StaticGroup(FILE* fp){
	if(fscanf(fp," %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg", 
		&(localTransform(0,0)),&(localTransform(1,0)),
		&(localTransform(2,0)),&(localTransform(3,0)),
		&(localTransform(0,1)),&(localTransform(1,1)),
		&(localTransform(2,1)),&(localTransform(3,1)),
		&(localTransform(0,2)),&(localTransform(1,2)),
		&(localTransform(2,2)),&(localTransform(3,2)),
		&(localTransform(0,3)),&(localTransform(1,3)),
		&(localTransform(2,3)),&(localTransform(3,3))) != 16){
		fprintf(stderr, "Failed to parse group_begin for StaticGroup\n"); 
		exit(EXIT_FAILURE);
	}
	shapeList=NULL;
}
StaticGroup::StaticGroup(Matrix m){
	localTransform=m;
	shapeList=NULL;
}

Matrix StaticGroup::getMatrix(void){return localTransform;}
void StaticGroup::writeBegin(int indent,FILE* fp){
	int i;
	for(i=0;i<indent;i++){fprintf(fp," ");}
	fprintf(fp,"#group_begin\n");

	for(i=0;i<indent;i++){fprintf(fp," ");}
	fprintf(fp,"  %lg %lg %lg %lg\n",
		localTransform(0,0),localTransform(1,0),
		localTransform(2,0),localTransform(3,0));
	for(i=0;i<indent;i++){fprintf(fp," ");}
	fprintf(fp,"  %lg %lg %lg %lg\n",
		localTransform(0,1),localTransform(1,1),
		localTransform(2,1),localTransform(3,1));
	for(i=0;i<indent;i++){fprintf(fp," ");}
	fprintf(fp,"  %lg %lg %lg %lg\n",
		localTransform(0,2),localTransform(1,2),
		localTransform(2,2),localTransform(3,2));
	for(i=0;i<indent;i++){fprintf(fp," ");}
	fprintf(fp,"  %lg %lg %lg %lg\n",
		localTransform(0,3),localTransform(1,3),
		localTransform(2,3),localTransform(3,3));
}
void StaticGroup::writeEnd(int indent,FILE* fp){
	int i;
	for(i=0;i<indent;i++){fprintf(fp," ");}
	fprintf(fp,"#group_end\n");
}

ParametrizedGroup::ParametrizedGroup(FILE* fp,char* parameterName){
	char type[100];
	if(fscanf(fp," %s %s ",type,parameterName) != 2){
		fprintf(stderr, "Failed to parse joint_begin for ParametrizedGroup\n"); 
		exit(EXIT_FAILURE);
	}
	if(strcmp(type,"TranslateX")==0){localTransform.setType(XTRANSLATE);}
	else if(strcmp(type,"TranslateY")==0){localTransform.setType(YTRANSLATE);}
	else if(strcmp(type,"TranslateZ")==0){localTransform.setType(ZTRANSLATE);}
	else if(strcmp(type,"RotateX")==0){localTransform.setType(XROTATE);}
	else if(strcmp(type,"RotateY")==0){localTransform.setType(YROTATE);}
	else if(strcmp(type,"RotateZ")==0){localTransform.setType(ZROTATE);}
	else if(strcmp(type,"ScaleX")==0){localTransform.setType(XSCALE);}
	else if(strcmp(type,"ScaleY")==0){localTransform.setType(YSCALE);}
	else if(strcmp(type,"ScaleZ")==0){localTransform.setType(ZSCALE);}
	else{
		fprintf(stderr, "Failed to parse joint type: (%s) for ParametrizedGroup\n",
			type);
		exit(EXIT_FAILURE);
	}
	parameter=NULL;
	shapeList=NULL;
}
Matrix ParametrizedGroup::getMatrix(void){
	return localTransform.getMatrix(parameter->value);
}
char* ParametrizedGroup::name(void){return "parametrized group";}

void ParametrizedGroup::writeBegin(int indent,FILE* fp){
	int i;
	for(i=0;i<indent;i++){fprintf(fp," ");}
	fprintf(fp,"#joint_begin\n");

	for(i=0;i<indent;i++){fprintf(fp," ");}
	switch(localTransform.getType()){
	case XTRANSLATE:
		fprintf(fp,"  TranslateX\n");
		break;
	case YTRANSLATE:
		fprintf(fp,"  TranslateY\n");
		break;
	case ZTRANSLATE:
		fprintf(fp,"  TranslateZ\n");
		break;
	case XROTATE:
		fprintf(fp,"  RotateX\n");
		break;
	case YROTATE:
		fprintf(fp,"  RotateY\n");
		break;
	case ZROTATE:
		fprintf(fp,"  RotateZ\n");
		break;
	case XSCALE:
		fprintf(fp,"  ScaleX\n");
		break;
	case YSCALE:
		fprintf(fp,"  ScaleY\n");
		break;
	case ZSCALE:
		fprintf(fp,"  ScaleZ\n");
		break;
	}
	for(i=0;i<indent;i++){fprintf(fp," ");}
	fprintf(fp,"%s\n",parameter->name);

}
void ParametrizedGroup::writeEnd(int indent,FILE* fp){
	int i;
	for(i=0;i<indent;i++){fprintf(fp," ");}
	fprintf(fp,"#joint_end\n");
}
