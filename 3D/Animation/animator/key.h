#ifndef KEY_INCLUDED
#define KEY_INCLUDED
#include <stdio.h>
#include "group.h"

#define KEY_FILE_NAME_SIZE 100
#define KEY_PARAMETER_NAME_SIZE 100
/* 
The Key class stores information about one degree of freedom.
It stores the number of samples, the sample values, and the name of the key.
*/ 
class Key{
	int s;
	char n[KEY_PARAMETER_NAME_SIZE];
	float* sampleValues;
public:
	class Parameter* parameter;

	Key(void);
	~Key(void);

	// Sets the name of the associated parameter
	void setParameterName(char* name);

	// Sets the number of samples points
	void setSize(int size);

	// Returns the number of sample points
	int samples(void);

	// Returns the name of the Key object
	char* name(void);

	// Provides a way for accessing the values of the sample points
	float& operator[](int index);
	float& index(int index);
};
/*
The KeyFile class stores the key frame information read out from the specified file
*/
class KeyFile{
	int kNum;
	int fNum;
	float d;
public:
	Key* keyList;
	char fileName[KEY_FILE_NAME_SIZE];

	KeyFile(char* fileName);
	~KeyFile(void);

	void read(char* fileName);
	void read(FILE* fp);
	void write(char* fileName);
	void write(FILE* fp=stdout);


	// Returns the number of keys
	int keyNum(void){return kNum;}

	// Returns the number of frames
	int frameNum(void){return fNum;}

	// Returns the duration
	float duration(void){return d;}

	// Provides a way to get the index of the specified key within the list
	int indexOf(char* name);

	// Writes the values for the specified time into the array
	void setParameterValues(float time);

	// Given a list of parameters, this method links them to the appropriate
	// Key objects
	void linkParameters(Parameter** parameterList,int pCount);
};

#endif /* KEY_INCLUDED */
  
