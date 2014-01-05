#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "bmp.h"
#include "scene.h"
#include "pointLight.h"
#include "directionalLight.h"
#include "spotLight.h"
#include "rayFileInstance.h"
#include "sphere.h"
#include "box.h"
#include "cone.h"
#include "cylinder.h"
#include "triangle.h"
#include "group.h"
#include "key.h"

#define BUF_SIZE 500
Material defaultMaterial(-1,Point3D(0.2,0.2,0.2),Point3D(0.8,0.8,0.8),1.0);

int Camera::read(FILE* fp){
	if(fscanf(fp," %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg",
		&(position[0]),&(position[1]),&(position[2]),
		&(direction[0]),&(direction[1]),&(direction[2]),
		&(up[0]),&(up[1]),&(up[2]),
		&(heightAngle)) !=10 ) {return 0;}
	direction=direction.unit();
	up=up.unit();
	right=direction.crossProduct(up);
	return 1;
}
void Camera::write(FILE* fp){
	fprintf(fp,"#camera\n");
	fprintf(fp,"   %lg %lg %lg\n",position[0],position[1],position[2]);
	fprintf(fp,"   %lg %lg %lg\n",direction[0],direction[1],direction[2]);
	fprintf(fp,"   %lg %lg %lg\n",up[0],up[1],up[2]);
	fprintf(fp,"   %lg\n",heightAngle);
}

int Vertex::read(FILE* fp){
	Flt size;
	if(fscanf(fp," %lg %lg %lg %lg %lg %lg %lg %lg",
		&(position[0]),&(position[1]),&(position[2]),
		&(normal[0]),&(normal[1]),&(normal[2]),
		&(texCoordinate[0]),&(texCoordinate[1])) != 8){return 0;}
	size=normal.length();
	if(size==0.0){printf("No normal specified for vertex\n");}
	else{normal=normal/size;}
	return 1;
}
void Vertex::write(FILE* fp){
	fprintf(fp,"#vertex\n");
	fprintf(fp,"   %lg %lg %lg\n",position[0],position[1],position[2]);
	fprintf(fp,"   %lg %lg %lg\n",normal[0],normal[1],normal[2]);
	fprintf(fp,"   %lg %lg\n",texCoordinate[0],texCoordinate[1]);
}

Material::Material(void){
	index=0;
	specular=diffuse=ambient=emissive=Point3D();
	kspec=0.0;
	ktran=0.0;
	refind=1.0;
	handle=0;
	tex=NULL;
	foo[0]=0;
}
Material::Material(int index, Point3D ambient, Point3D color, Flt kspec){
	this->index=index;
	this->ambient=ambient;
	this->specular=this->diffuse=color;
	this->emissive=Point3D();
	this->kspec=kspec;
	this->ktran=0.0;
	this->refind=1.0;
	this->handle=0;
	this->tex=NULL;
	this->foo[0]=0;
}
int Material::read(FILE* fp,int& temp){
  if(fscanf(fp," %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %d !%s!",
	    &(ambient[0]),&(ambient[1]),&(ambient[2]),
	    &(diffuse[0]),&(diffuse[1]),&(diffuse[2]),
	    &(specular[0]),&(specular[1]),&(specular[2]),
	    &(emissive[0]),&(emissive[1]),&(emissive[2]),
	    &(kspec),&(ktran),&(refind),
	    &temp,
	    foo) != 17){return 0;}
  else{
    if(foo[strlen(foo)-1]=='!'){foo[strlen(foo)-1]='\0';}
    return 1;
  }
}
void Material::write(FILE* fp){
	int i;
	if(tex==NULL){i=-1;}
	else{i=tex->index;}
	fprintf(fp,"#material\n");
	fprintf(fp,"   %lg %lg %lg\n",ambient[0],ambient[1],ambient[2]);
	fprintf(fp,"   %lg %lg %lg\n",diffuse[0],diffuse[1],diffuse[2]);
	fprintf(fp,"   %lg %lg %lg\n",specular[0],specular[2],specular[2]);
	fprintf(fp,"   %lg %lg %lg\n",emissive[0],emissive[1],emissive[2]);
	fprintf(fp,"   %lg %lg %lg\n",kspec,ktran,refind);
	fprintf(fp,"   %d\n",i);
	fprintf(fp,"   !%s!\n",foo);
}
void Material::setHandle(void){
	GLfloat m_ambient[4]= { 0.0, 0.0, 0.0, 1.0 };
	GLfloat m_diffuse[4]= { 0.0, 0.0, 0.0, 1.0 };
	GLfloat m_specular[4]= {0.0, 0.0, 0.0, 1.0 };
	GLfloat m_emissive[4]= { 0.0, 0.0, 0.0, 1.0 };
	GLfloat m_shininess=  0.0;

	if(handle!=0){return;}
	m_ambient[0] = ambient[0];
	m_ambient[1] = ambient[1];
    m_ambient[2] = ambient[2];		

    m_diffuse[0] = diffuse[0];
    m_diffuse[1] = diffuse[1];
    m_diffuse[2] = diffuse[2];

    m_specular[0] = specular[0];	
    m_specular[1] = specular[1];	
    m_specular[2] = specular[2];				

    m_emissive[0] = emissive[0];	
    m_emissive[1] = emissive[1];	
    m_emissive[2] = emissive[2];	
    
    m_shininess = 128.0 * kspec;	

    handle=glGenLists(1);
    glNewList(handle,GL_COMPILE);
    if(tex==NULL){glDisable(GL_TEXTURE_2D);}
    else{
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D,tex->handle);
    }
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, m_ambient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, m_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, m_specular);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, m_emissive);
    glMaterialf( GL_FRONT_AND_BACK,GL_SHININESS, m_shininess);
    glEndList();                      
}

Texture::Texture(void){
	index=0;
	handle=0;
	filename[0]=0;
	img=NULL;
}
Texture::~Texture(void){if(img!=NULL){delete img;}}


int Texture::read(FILE* f){
	FILE *fp;

	if(fscanf(f," %s",filename) != 1){return 0;}
	fp=fopen(filename,"rb");
	if(fp==NULL){return 0;}
	img=BMPReadImage(fp);
	if(fclose(fp) != 0){return 0;}
	return 1;
}
void Texture::write(FILE* fp){
	fprintf(fp,"#texture\n");
	fprintf(fp,"   %s\n",filename);
}
void Texture::setHandle(void){
	if(handle!=0){return;}
	glGenTextures(1,&handle);
    glBindTexture(GL_TEXTURE_2D,handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
		    GL_LINEAR_MIPMAP_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,img->width(),img->height(),
		      GL_RGB,GL_UNSIGNED_BYTE,img->pixelList);

}
RayFile::~RayFile(void){if(scene!=NULL){delete scene;}}
int RayFile::read(FILE* fp){
	scene=new Scene();
	assert(scene);
	if(fscanf(fp," %s",filename) != 1){return 0;} 
	scene->read(filename);
	return 1;
}
void RayFile::write(FILE* fp){
	fprintf(fp,"#ray_file\n");
	fprintf(fp,"   %s\n",filename);
}
Scene::Scene(void){
	ambient=Point3D(0.0,0.0,0.0);
	background=Point3D(0.0,0.0,0.0);
	lights=NULL;
	lightNum=0;
	materials=NULL;
	materialNum=0;
	textures=NULL;
	textureNum=0;
	rayFiles=NULL;
	rayFileNum=0;
	keyFileNum=0;
	keyFiles=NULL;
	parameters=NULL;
	parameterNum=0;
}
Material* Scene::getMaterial(int index){
	assert(index>=0 && index<materialNum);
	return &(materials[index]);
}

Scene::~Scene(void){
	int i;

	for(i=0;i<lightNum;i++){delete lights[i];}
	if(lightNum>0){delete[] lights;}
	if(materialNum>0){delete[] materials;}
	if(textureNum>0){delete[] textures;}
	if(vertexNum>0){delete[] vertices;}
	if(rayFileNum>0){delete[] rayFiles;}
	if(group!=NULL){delete group;}
	if(camera!=NULL){delete camera;}
	for(i=0;i<parameterNum;i++){delete parameters[i];}
	if(parameterNum>0){delete[] parameters;}
	for(i=0;i<keyFileNum;i++){delete keyFiles[i];}
	if(keyFileNum>0){delete[] keyFiles;}
}
void Scene::write(FILE* fp){
	int i;
	ShapeListElement* temp;

	if(camera!=NULL){camera->write(fp);}
	if(ambient[0]!=0.0 || ambient[1]!=0.0 || ambient[2]!=0.0){
		fprintf(fp,"#ambient\n");
		fprintf(fp,"   %lg %lg %lg\n",ambient[0],ambient[1],ambient[2]);
	}
	if(background[0]!=0.0 || background[1]!=0.0 || background[2]!=0.0){
		fprintf(fp,"#background\n");
		fprintf(fp,"   %lg %lg %lg\n",background[0],background[1],background[2]);
	}
	if(lightNum!=0){
		fprintf(fp,"#light_num\n");
		fprintf(fp,"   %d\n",lightNum);
		for(i=0;i<lightNum;i++){lights[i]->write(fp);}
	}
	if(textureNum!=0){
		fprintf(fp,"#texture_num\n");
		fprintf(fp,"   %d\n",textureNum);
		for(i=0;i<textureNum;i++){textures[i].write(fp);}
	}
	if(materialNum!=0){
		fprintf(fp,"#material_num\n");
		fprintf(fp,"   %d\n",materialNum);
		for(i=0;i<materialNum;i++){materials[i].write(fp);}
	}
	if(vertexNum!=0){
		fprintf(fp,"#vertex_num\n");
		fprintf(fp,"   %d\n",vertexNum);
		for(i=0;i<vertexNum;i++){vertices[i].write(fp);}
	}
	if(rayFileNum!=0){
		fprintf(fp,"#ray_file_num\n");
		fprintf(fp,"   %d\n",rayFileNum);
		for(i=0;i<rayFileNum;i++){rayFiles[i].write(fp);}
	}
	if(keyFileNum>0){
		for(i=0;i<keyFileNum;i++){
			fprintf(fp,"#key_file\n");
			fprintf(fp,"   %s\n",keyFiles[i]->fileName);
		}
	}
	if(!group->localTransform.isIdentity()){group->write(0,fp);}
	else{
		temp=group->shapeList;
		while(temp!=NULL){
			temp->shape->write(0,fp);
			temp=temp->next;
		}
	}
}

void Scene::setHandles(void){
	int i;

	for(i=0;i<textureNum;i++){textures[i].setHandle();}
	for(i=0;i<materialNum;i++){materials[i].setHandle();}
	defaultMaterial.setHandle();
	for(i=0;i<rayFileNum;i++){rayFiles[i].scene->setHandles();}
}
void Scene::setParameters(float time){
	int i;
	for(i=0;i<keyFileNum;i++){keyFiles[i]->setParameterValues(time);}
	for(i=0;i<rayFileNum;i++){rayFiles[i].scene->setParameters(time);}
}

void Scene::read(const char* fileName){
	char keyword[BUF_SIZE];
	char keyFileName[BUF_SIZE];
	int cmndCtr=0;
	int backFlag=0, ambFlag=0;
	int indL=-1, indM=-1, indT=-1,indRF=-1,indV=-1;
	int temp,i;
	int inShapes=0;
	Group* root;
	FILE* fp;
	KeyFile** tempArray;

	fp=fopen(fileName,"r");
	printf("trying to open %s\n",fileName);
	assert(fp);
	group=new StaticGroup(IdentityMatrix());
	root=group;

	lightNum=-1;
	textureNum=-1;
	materialNum=-1;
	rayFileNum=-1;
	vertexNum=-1;
	parameterNum=0;
	parameters=NULL;
	lights=NULL;
	textures=NULL;
	materials=NULL;
	rayFiles=NULL;
	vertices=NULL;
	camera=NULL;
	keyFileNum=0;
	keyFiles=NULL;


	while(fscanf(fp," #%s",keyword) == 1){
		cmndCtr++;

//We try to read out the camera information. We make sure that we have
//not begun describing the scene graph yet, and we ignore the camera if 
//it is the second one specified.

		if(strcmp(keyword,"camera") == 0){
			if (inShapes!=0){
				ParseLineError(cmndCtr,fileName,
					"not a valid directive within a group\n");
			}
			if(camera!=NULL){
				fprintf(stderr,"Parsing Line %d: Ignoring extra camera\n",cmndCtr);
			}
			else {
				camera=new Camera;
				if(camera==NULL){
					ParseLineError(cmndCtr,fileName,
						"failed to allocate camera memory\n");
				}
				if(camera->read(fp)==0){
					ParseLineError(cmndCtr,fileName,
						"failed to parse camera settings\n");
				}
			}
		}

//Next we try to read out the environment information. Again, we make
//sure that we have not begun describing the scene graph yet and we 
//ignore all but the first background color and ambient color 
//specifications.

		else if(strcmp(keyword,"background") == 0){
			if(inShapes!=0){
				ParseLineError(cmndCtr,fileName,
					"not a valid directive within a group\n");
			}
			if(backFlag==1){
				fprintf(stderr,"Parsing Line %d: Ignoring extra background\n",cmndCtr);
			}
			else {
				backFlag=1;
				if(fscanf(fp," %lg %lg %lg",
					&(background[0]),&(background[1]),&(background[2])) != 3){
					ParseLineError(cmndCtr,fileName,
						"failed to parse background settings\n");
				}
			}
		}
		else if(strcmp(keyword,"ambient") == 0){
			if(inShapes!=0){
				ParseLineError(cmndCtr,fileName,
					"not a valid directive within a group\n");
			}
			if(ambFlag==1){
				fprintf(stderr,"Parsing Line %d: Ignoring extra ambient\n",cmndCtr);
			}
			else {
				ambFlag=1;
				if(fscanf(fp," %lg %lg %lg",
					&(ambient[0]),&(ambient[1]),&(ambient[2])) != 3){
					ParseLineError(cmndCtr,fileName,
						"failed to parse ambient settings\n");
				}
			}
		}
//Next we try to read out the .key file. We make sure that we have not
//begun describing the scene graph.
		else if(strcmp(keyword,"key_file") == 0){
			if(inShapes!=0){
				ParseLineError(cmndCtr,fileName,
					"not a valid directive within a group\n");
			}
			keyFileNum++;
			tempArray=new KeyFile*[keyFileNum];
			assert(tempArray);
			for(i=0;i<keyFileNum-1;i++){tempArray[i]=keyFiles[i];}
			fscanf(fp," %s",keyFileName);
			tempArray[i]=new KeyFile(keyFileName);
			assert(tempArray[i]);
			delete[] keyFiles;
			keyFiles=tempArray;
		}
		else if(strcmp(keyword,"ambient") == 0){
			if(inShapes!=0){
				ParseLineError(cmndCtr,fileName,
					"not a valid directive within a group\n");
			}
			if(ambFlag==1){
				fprintf(stderr,"Parsing Line %d: Ignoring extra ambient\n",cmndCtr);
			}
			else {
				ambFlag=1;
				if(fscanf(fp," %lg %lg %lg",
					&(ambient[0]),&(ambient[1]),&(ambient[2])) != 3){
					ParseLineError(cmndCtr,fileName,
						"failed to parse ambient settings\n");
				}
			}
		}

//Next we try read out the number of lights. We make sure that we have
//not begun describing the scene graph, and we make sure that the number
//of lights is only specified once. Once we have a "good" light number
//we allocate memory for the lights 

		else if(strcmp(keyword,"light_num") == 0){
			if (inShapes!=0){
				ParseLineError(cmndCtr,fileName,
					"not a valid directive within a group\n");
			}
			if(lightNum>-1){
				ParseLineError(cmndCtr,fileName,
					"attempted to define light_num more than once\n");
			}
			else{
				if (fscanf(fp," %d", &(lightNum)) != 1){
					ParseLineError(cmndCtr,fileName,
						"failed to parse num_lights setting\n");
				}
				else{
					if(lightNum < 0){
						ParseLineError(cmndCtr,fileName,
							"you can't have negative number of lights\n");
					}
					else if(lightNum == 0){lights=NULL;}
					else {
						lights=new Light*[lightNum];
						if(lights == NULL){
							ParseLineError(cmndCtr,fileName,
								"failed to allocate memory Lights\n");
						}
					}
				}
			}
		}

//Next we try read out the number of textures. We make sure that we have
//not begun describing the scene graph, and we make sure that the number
//of textures is only specified once. Once we have a "good" texture number
//we allocate memory for the textures.

	    else if(strcmp(keyword,"texture_num") == 0){
			if(inShapes!=0){
				ParseLineError(cmndCtr,fileName,
					"not a valid directive within a group\n");
			}
			if(textureNum>-1){
				ParseLineError(cmndCtr,fileName,
					"attempted to define texture_num more than once\n");
			}
			else{
				if(fscanf(fp," %d",&(textureNum)) != 1){
					ParseLineError(cmndCtr,fileName,
						"failed to parse num_textures setting\n");
				}
				else{
					if(textureNum < 0){
						ParseLineError(cmndCtr,fileName,
							"you can't have negative number of textures\n");
					}
					else if(textureNum == 0){textures=NULL;}
					else{
						textures=new Texture[textureNum];
						if(textures == NULL){
							ParseLineError(cmndCtr,fileName,
								"failed to allocate memory for textures\n");
						}
					}
				}
			}
		}

//Next we try read out the number of materials. We make sure that we have
//not begun describing the scene graph, and we make sure that the number
//of materials is only specified once. Once we have a "good" material
//number we allocate memory for the materials.

		else if(strcmp(keyword,"material_num") == 0){
			if (inShapes!=0){
				ParseLineError(cmndCtr,fileName,
					"not a valid directive within a group\n");
			}
			if(materialNum>-1){
				ParseLineError(cmndCtr,fileName,
					"attempted to define material_num more than once\n");
			}
			else{
				if(fscanf(fp," %d",&(materialNum)) != 1){
					ParseLineError(cmndCtr,fileName,
						"failed to parse num_materials setting\n");
				}
				else {
					if(materialNum < 0){
						ParseLineError(cmndCtr,fileName,
							"you can't have negative number of materials\n");
					}
					else{
						if(materialNum>0){
							materials=new Material[materialNum];
							if(materials == NULL){
								ParseLineError(cmndCtr,fileName,
									"failed to allocate memory for materials\n");
							}
						}
						else{materials=NULL;}
					}
				}
			}
		}

//Next we try read out the number of vertices. We make sure that we have
//not begun describing the scene graph, and we make sure that the number
//of vertices is only specified once. Once we have a "good" vertex
//number we allocate memory for the vertices.

		else if(strcmp(keyword,"vertex_num") == 0){
			if (inShapes!=0){
				ParseLineError(cmndCtr,fileName,
					"not a valid directive within a group\n");
			}
			if(vertexNum>-1){
				ParseLineError(cmndCtr,fileName,
					"attempted to define material_num more than once\n");
			}
			else{
				if(fscanf(fp," %d",&(vertexNum)) != 1){
					ParseLineError(cmndCtr,fileName,
						"failed to parse num_materials setting\n");
				}
				else {
					if(vertexNum < 0){
						ParseLineError(cmndCtr,fileName,
							"you can't have negative number of materials\n");
					}
					else{
						if(vertexNum>0){
							vertices=new Vertex[vertexNum];
							if(vertices == NULL){
								ParseLineError(cmndCtr,fileName,
									"failed to allocate memory for vertices\n");
							}
						}
						else{vertices=NULL;}
					}
				}
			}
		}

//Next we try read out the number of ray files. We make sure that we have
//not begun describing the scene graph, and we make sure that the number
//of ray files is only specified once. Once we have a "good" ray file
//number we allocate memory for the ray files.

		else if(strcmp(keyword,"ray_file_num") == 0){
			if (inShapes!=0){
				ParseLineError(cmndCtr,fileName,
					"not a valid directive within a group\n");
			}
			if(rayFileNum>-1){
				ParseLineError(cmndCtr,fileName,
					"attempted to define ray_file_num more than once\n");
			}
			else{
				if(fscanf(fp," %d",&(rayFileNum)) != 1){
					ParseLineError(cmndCtr,fileName,
						"failed to parse ray_file_num setting\n");
				}
				else {
					if(rayFileNum<=0){rayFiles=NULL;}
					else{
						rayFiles=new RayFile[rayFileNum];
						if(rayFiles == NULL){
							ParseLineError(cmndCtr,fileName,
								"failed to allocate memory for ray_files\n");
						}
					}
				}
			}
		}

//Next we try read out the next textures. We make sure that we have
//not begun describing the scene graph. We make sure that we are not
//adding more textures then we promised and we make sure that all the
//file I/O works as it should.

		else if(strcmp(keyword,"texture") == 0){
			if(inShapes!=0){
				ParseLineError(cmndCtr,fileName,
					"not a valid directive within a group\n");
			}
			if(textureNum == -1){
				ParseLineError(cmndCtr,fileName,
					"attempted to define texture before texture_num\n");
			}
			indT++;
			if(indT == textureNum){
				ParseLineError(cmndCtr,fileName,
					"attempted to define more textures than declared\n");
			}
			if(textures[indT].read(fp)==0){
				ParseLineError(cmndCtr,fileName,"failed to generate texture\n");
			}
			textures[indT].index=indT;
		}

//Next we try read out the next material. We make sure that we have
//not begun describing the scene graph. We make sure that we are not
//adding more materials then we promised.

		else if(strcmp(keyword,"material") == 0){
			if(inShapes!=0){
				ParseLineError(cmndCtr,fileName,
					"not a valid directive within a group\n");
			}
			if(materialNum==-1){
				ParseLineError(cmndCtr,fileName,
					"attempted to define materials before material_num\n");
			}
			indM++;
			if(indM == materialNum){
				ParseLineError(cmndCtr,fileName,
					"attempted to define more materials than declared\n");
			}
			else{
				if(materials[indM].read(fp,temp)==0){
					ParseLineError(cmndCtr,fileName,"failed to parse material\n");
				}
				else{
					if(temp==-1){materials[indM].tex=NULL;}
					else{
						if(temp >= textureNum){
							ParseLineError(cmndCtr,fileName,
								"material specifies a texture out of texture_num bounds (make sure texture_num comes before material_num) \n");
						}
						materials[indM].tex=&(textures[temp]);
					}
					materials[indM].index=indM;
				}
			}
		}

//Next we try read out the next vertex. We make sure that we have
//not begun describing the scene graph. We make sure that we are not
//adding more vertices then we promised.

		else if(strcmp(keyword,"vertex") == 0){
			if(inShapes!=0){
				ParseLineError(cmndCtr,fileName,
					"not a valid directive within a group\n");
			}
			if(vertexNum==-1){
				ParseLineError(cmndCtr,fileName,
					"attempted to define vertices before vertex_num\n");
			}
			indV++;
			if(indV == vertexNum){
				ParseLineError(cmndCtr,fileName,
					"attempted to define more vertices than declared\n");
			}
			else{
				if(vertices[indV].read(fp)==0){
					ParseLineError(cmndCtr,fileName,"failed to parse vertices\n");
				}
				else{vertices[indV].index=indV;}
			}
		}
//Next we try to read out the next rayFile
		else if(strcmp(keyword,"ray_file") == 0){
			if(inShapes!=0){
				ParseLineError(cmndCtr,fileName,
					"not a valid directive within a group\n");
			}
			if(rayFileNum==-1){
				ParseLineError(cmndCtr,fileName,
					"attempted to define ray_files before ray_file_num\n");
			}
			indRF++;
			if(indRF == rayFileNum){
				ParseLineError(cmndCtr,fileName,
					"attempted to define more ray_files than declared\n");
			}
			else{
				if(rayFiles[indRF].read(fp)==0){
					ParseLineError(cmndCtr,fileName,"failed to parse ray_file\n");
				}
				rayFiles[indRF].index=indRF;
			}
		}

//Next we try read out the next light. We make sure that we have
//not begun describing the scene graph. We make sure that we are not
//adding more materials then we promised.

		else if(strcmp(keyword,"light_point") == 0 || 
			strcmp(keyword,"light_dir")  == 0 ||
			strcmp(keyword,"light_spot") == 0){
			if(inShapes!=0){
				ParseLineError(cmndCtr,fileName,
					"not a valid directive within a group\n");
			}
			if(lightNum==-1){
				ParseLineError(cmndCtr,fileName,
					"attempted to define lights before light_num\n");
			}
			indL++;
			if(indL == lightNum){
				ParseLineError(cmndCtr,fileName,
					"attempted to define more lights than light_num\n");
			}
			else{
				if(strcmp(keyword,"light_point")==0){ 
					lights[indL]=new PointLight;
				}
				else if(strcmp(keyword,"light_dir")==0){
					lights[indL]=new DirectionalLight;
				}
				else if(strcmp(keyword,"light_spot")==0){
					lights[indL]=new SpotLight;
				}
				if(lights[indL]==NULL){
					ParseLineError(cmndCtr,fileName,
						"failed to allocate memory for light\n");
				}
				if(lights[indL]->read(fp) == 0){
					ParseLineError(cmndCtr,fileName,"failed to parse light\n");
				}
			}
		}

//Next we try read out the scene graph information, obtaining the 
//transform and either creating a new scene graph or adding a node
//to the existing one.

		else if(strcmp(keyword,"group_begin") == 0){
			inShapes=1;
			ParseGroup(fp,cmndCtr,root,fileName);
		}
		else if(strcmp(keyword,"joint_begin") == 0){
			inShapes=1;
			ParseJoint(fp,cmndCtr,root,fileName);
		}

//We check if we have come to the end of a scene-graph group. If we have
//this is bad because we have not yet opened one

		else if(strcmp(keyword,"group_end") == 0){
			ParseLineError(cmndCtr,fileName,
				"closed more groups then began\n");
		}
		else if(strcmp(keyword,"joint_end") == 0){
			ParseLineError(cmndCtr,fileName,
				"closed more groups then began\n");
		}
		else if(ParseShape(fp,keyword,cmndCtr,root,fileName)){inShapes=1;}
		else{
			fprintf(stderr,
				"Ignoring Command %d: No valid #directive...%s \n",cmndCtr,
				keyword);
		}		
	}		
	if(indL<(lightNum-1)){
		ParseError("Couldn't find enough lights for light_num in",fileName);
	}
	if(indM<(materialNum-1)){
		ParseError("Couldn't find enough materials for material_num in",fileName);
	}
	if(indT<(textureNum-1)){
		ParseError("Couldn't find enough textures for texture_num in",fileName);
	}
	if(indRF<(rayFileNum-1)){
		ParseError("Couldn't find enough ray_files for ray_file_num in",fileName);
	}
	if(lightNum==-1){lightNum=0;}
	if(materialNum==-1){materialNum=0;}
	if(textureNum==-1){textureNum=0;}
	if(rayFileNum==-1){rayFileNum=0;}
	if(vertexNum==-1){vertexNum=0;}
	fclose(fp);
	for(i=0;i<keyFileNum;i++){
		keyFiles[i]->linkParameters(parameters,parameterNum);
	}
}
void Scene::ParseGroup(FILE* fp,int& cmndCtr,Group* current,const char* fileName){
	char keyword[BUF_SIZE];
	Group *currNode,*newNode;
	currNode=current;

	newNode=new StaticGroup(fp);
	if(newNode==NULL){
		ParseLineError(cmndCtr,fileName,"failed to allocate memory for StaticGroup\n");
	}
	currNode->addShape(newNode);
	currNode=newNode;

	while(fscanf(fp," #%s",keyword) == 1){
		cmndCtr++;
		if(strcmp(keyword,"group_begin") == 0){
			ParseGroup(fp,cmndCtr,currNode,fileName);
		}
		else if(strcmp(keyword,"joint_begin") == 0){
			ParseJoint(fp,cmndCtr,currNode,fileName);
		}
    /* We check if we have come to the end of a scene-graph group and we
     * do the appropriate pointer manipulation, making sure that we are not
     * leaving a group we had not begun */
		else if(strcmp(keyword,"group_end") == 0){return;}
		else if(strcmp(keyword,"joint_end")==0){
			ParseLineError(cmndCtr,fileName,"joint_end within a group\n");
		}
		else if(ParseShape(fp,keyword,cmndCtr,currNode,fileName)){;}
		else {
			fprintf(stderr,
				"Ignoring Command %d: No valid #directive...%s \n",cmndCtr,
				keyword);
		}
	}
	ParseLineError(cmndCtr,fileName,"no group_end\n");
}
void Scene::ParseJoint(FILE* fp,int& cmndCtr,Group* current,const char* fileName){
	char keyword[BUF_SIZE];
	char parameterName[BUF_SIZE];
	ParametrizedGroup *newNode;
	Group *currNode;
	int i;
	Parameter** tempArray;

	currNode=current;

	newNode=new ParametrizedGroup(fp,parameterName);
	if(newNode==NULL){
		ParseLineError(cmndCtr,fileName,
			"failed to allocate memory for ParametrizedGroup\n");
	}
	for(i=0;i<parameterNum;i++){
		if(strcmp(parameterName,parameters[i]->name)==0){break;}
	}
	if(i==parameterNum){
		parameterNum++;
		tempArray=new Parameter*[parameterNum];
		assert(tempArray);
		for(i=0;i<parameterNum-1;i++){tempArray[i]=parameters[i];}
		tempArray[parameterNum-1]=new Parameter();
		strcpy(tempArray[i]->name,parameterName);
		tempArray[i]->value=newNode->localTransform.defaultParameterValue();
		if(parameterNum>1){delete[] parameters;}
		parameters=tempArray;
		newNode->parameter=parameters[parameterNum-1];
	}
	else{newNode->parameter=parameters[i];}
	currNode->addShape(newNode);
	currNode=newNode;

	while(fscanf(fp," #%s",keyword) == 1){
		cmndCtr++;
		if(strcmp(keyword,"group_begin") == 0){
			ParseGroup(fp,cmndCtr,currNode,fileName);
		}
		else if(strcmp(keyword,"joint_begin") == 0){
			ParseJoint(fp,cmndCtr,currNode,fileName);
		}
    /* We check if we have come to the end of a scene-graph group and we
     * do the appropriate pointer manipulation, making sure that we are not
     * leaving a group we had not begun */
		else if(strcmp(keyword,"joint_end") == 0){return;}
		else if(strcmp(keyword,"group_end")==0){
			ParseLineError(cmndCtr,fileName,"group_end within a joint\n");
		}
		else if(ParseShape(fp,keyword,cmndCtr,currNode,fileName)){;}
		else {
			fprintf(stderr,
				"Ignoring Command %d: No valid #directive...%s \n",cmndCtr,
				keyword);
		}
	}
	ParseLineError(cmndCtr,fileName,"no group_end\n");
}

int Scene::ParseShape(FILE* fp,const char* keyword,int cmndCtr,Group* curr,
		      const char* fileName){
	int temp;
	Group* current=curr;
	RayFileInstance* rayFileInstance;
	Sphere* sphere;
	Cone* cone;
	Cylinder* cylinder;
	Triangle* triangle;
	Box* box;

  /* If we are reading in new files we want them added to the current node
   * and we want to store there information in a temporary scene object we
   * will conveniently forget to free up. */

	if(strcmp(keyword,"ray_file_instance") == 0){
		if(fscanf(fp," %d",&temp) != 1){
			ParseLineError(cmndCtr,fileName,
				"could not read ray_file index number\n");
		}
		if(temp<0){
			ParseLineError(cmndCtr,fileName,
				"you cannot have negative ray_file index\n");
		}
		if(temp>=rayFileNum){
			ParseLineError(cmndCtr,fileName,
				"shape specifies a ray_file out of ray_file_num bounds (make sure ray_file_num comes before the root group) \n");
		}
		rayFileInstance=new RayFileInstance(&(rayFiles[temp]));
		if(rayFileInstance==NULL){
			ParseLineError(cmndCtr,fileName,"failed to allocate memory for Shape\n");
		}
		current->addShape(rayFileInstance);
		return 1;
	}
  /* We start reading in the shapes. We check that we are inside
   * a group and we call the appropriate instantiator */	
 
	else if(strcmp(keyword,"shape_sphere") == 0){
		sphere=new Sphere(fp,&temp);
		if(sphere == NULL){
			ParseLineError(cmndCtr,fileName,
				"failed to allocate memory for Sphere\n");
		}
		if(temp<-1){
			ParseLineError(cmndCtr,fileName,
				"you cannot have a negative material index\n");
		}
		if(temp!=-1 && temp >= materialNum){
			ParseLineError(cmndCtr,fileName,
				"shape specifies a material out of material_num bounds (make sure material_num comes before the root group) \n");
		}
		if(temp==-1){sphere->material=&defaultMaterial;}
		else{sphere->material=&(materials[temp]);}
		current->addShape(sphere);
		return 1;
	}
	
	else if(strcmp(keyword,"shape_box") == 0){
		box=new Box(fp,&temp);
		if (box == NULL){
			ParseLineError(cmndCtr,fileName,"failed to allocate memory for Box\n");
		}
		if(temp<-1){
			ParseLineError(cmndCtr,fileName,
				"you cannot have a negative material index\n");
		}
		if(temp!=-1 && temp >= materialNum){
			ParseLineError(cmndCtr,fileName,
				"shape specifies a material out of material_num bounds (make sure material_num comes before the root group) \n");
		}
		if(temp==-1){box->material=&defaultMaterial;}
		else{box->material=&(materials[temp]);}
		current->addShape(box);
		return 1;
	}
    
	else if(strcmp(keyword,"shape_cone") == 0){
		cone=new Cone(fp,&temp);
		if(cone == NULL){
			ParseLineError(cmndCtr,fileName,"failed to allocate memory for Cone\n");
		}
		if(temp<-1){
			ParseLineError(cmndCtr,fileName,
				"you cannot have a negative material index\n");
		}
		if(temp!=-1 && temp >= materialNum){
			ParseLineError(cmndCtr,fileName,
				"shape specifies a material out of material_num bounds (make sure material_num comes before the root group) \n");
		}
		if(temp==-1){cone->material=&defaultMaterial;}
		else{cone->material=&(materials[temp]);}
		current->addShape(cone);
		return 1;
	}

	else if(strcmp(keyword,"shape_cylinder") == 0){
		cylinder=new Cylinder(fp,&temp);
		if (cylinder == NULL){
			ParseLineError(cmndCtr,fileName,
				"failed to allocate memory for Cylinder\n");
		}
		if(temp<-1){
			ParseLineError(cmndCtr,fileName,
				"you cannot have a negative material index\n");
		}
		if(temp!=-1 && temp >= materialNum){
			ParseLineError(cmndCtr,fileName,
				"shape specifies a material out of material_num bounds (make sure material_num comes before the root group) \n");
		}
		if(temp==-1){cylinder->material=&defaultMaterial;}
		else{cylinder->material=&(materials[temp]);}
		current->addShape(cylinder);
		return 1;
	}
	else if(strcmp(keyword,"shape_triangle") == 0){
		triangle=new Triangle(fp,&temp,vertices,vertexNum);
		if(triangle == NULL){
			ParseLineError(cmndCtr,fileName,
				"failed to allocate memory for Triangle\n");
		}
		if(temp<-1){
			ParseLineError(cmndCtr,fileName,
				"you cannot have a negative material index\n");
		}
		if(temp!=-1 && temp >= materialNum){
			ParseLineError(cmndCtr,fileName,
				"shape specifies a material out of material_num bounds (make sure material_num comes before the root group) \n");
		}
		if(temp==-1){triangle->material=&defaultMaterial;}
		else{triangle->material=&(materials[temp]);}
		current->addShape(triangle);
		return 1;
	}
	return 0;
}		


/*////////////////////////////////////////////////////*/

void Scene::ParseError(const char *s1, const char *s2){
  fprintf(stderr, "Parsing error: %s %s\n", s1, s2);
  exit(EXIT_FAILURE);
}

void Scene::ParseLineError(int i,const char* fileName, const char *s){
  fprintf(stderr, "%s: Parsing error of command %d: %s\n",fileName,i,s);
  exit(EXIT_FAILURE);
}
