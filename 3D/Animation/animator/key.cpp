#include <string.h>
#include <assert.h>
#include "key.h"


// Key Class definitions
Key::Key(void){
	s=0;
	sampleValues=NULL;
	parameter=NULL;
}
Key::~Key(void){if(s>0){delete[] sampleValues;}}
void Key::setParameterName(char* name){strcpy(n,name);}

void Key::setSize(int size){
	assert(size>0);
	if(s>0){delete[] sampleValues;}
	s=size;
	sampleValues=new float[s];
	assert(sampleValues);
}
int Key::samples(void){return s;}

char* Key::name(void){return n;}

float& Key::index(int i){
	assert(0<=i && i<s);
	return sampleValues[i];
}

float& Key::operator[](int i){return index(i);}

// KeyFile Class definitions
KeyFile::KeyFile(char* fileName){
	strcpy(this->fileName,fileName);
	read(this->fileName);
}
KeyFile::~KeyFile(void){if(kNum>0){delete[] keyList;}}

int KeyFile::indexOf(char* n){
	int i;
	for(i=0;i<kNum;i++){
		if(strcmp(keyList[i].name(),n)==0){return i;}
	}
	return -1;
}
void KeyFile::read(char* fileName){
	FILE* fp=fopen(fileName,"r");
	assert(fp);
	read(fp);
	fclose(fp);
}
void KeyFile::read(FILE* fp){
	int i,j;
	char temp[100];
	float t=0.0;

	// Read in the degrees of freedom and set the size of the key list appropriately
	assert(fscanf(fp," #DOFS %d ",&kNum)==1);
	assert(kNum>0);
	keyList=new Key[kNum];
	assert(keyList);

	// Read in the names of the keys and create new keys
	for(i=0;i<kNum;i++){
		assert(fscanf(fp," %s",temp)==1);
		keyList[i].setParameterName(temp);
	}

	// Read in the duration
	assert(fscanf(fp," #DURATION %f ",&d)==1);
	assert(d>0);

	// Read in the number of frames and set the key sizes appropriately
	assert(fscanf(fp," #FRAMES %d ",&fNum)==1);
	assert(fNum>0);
	for(i=0;i<kNum;i++){keyList[i].setSize(fNum);}

	// Set the key values
	for(i=0;i<fNum;i++){
		for(j=0;j<kNum;j++){
			assert(fscanf(fp," %f ",&t)==1);
			keyList[j][i]=t;
		}
	}
}
void KeyFile::write(char* fileName){
	FILE* fp=fopen(fileName,"w");
	assert(fp);
	write(fp);
	fclose(fp);
}
void KeyFile::write(FILE* fp){
	int i,j;
	fprintf(fp,"#DOFS %d",kNum);
	for(i=0;i<kNum;i++){fprintf(fp," %s",keyList[i].name());}
	fprintf(fp,"\n");
	fprintf(fp,"#DURATION %f\n",d);
	fprintf(fp,"#FRAMES %d\n",fNum);
	for(i=0;i<fNum;i++){
		for(j=0;j<kNum;j++){
			fprintf(fp," %f",keyList[j][i]);
		}
		fprintf(fp,"\n");
	}
}
void KeyFile::setParameterValues(float time){
	int i,j;
	float t,e;

	t=time/d;
	t-=(int)t;
	t*=fNum;
	j=(int)t;
	e=t-(float)j;
	for(i=0;i<kNum;i++){
		keyList[i].parameter->value=keyList[i][0];
	}
}
void KeyFile::linkParameters(Parameter** parameterList,int pCount){
	int i,j;
	for(i=0;i<pCount;i++){
		for(j=0;j<kNum;j++){
			if(strcmp(parameterList[i]->name,keyList[j].name())==0){
				keyList[j].parameter=parameterList[i];
			}
		}
	}
}
