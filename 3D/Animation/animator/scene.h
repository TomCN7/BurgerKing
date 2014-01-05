#ifndef SCENE_INCLUDED
#define SCENE_INCLUDED
#include "glut.h"
#include "geometry.h"
#include "bmp.h"
#include "shape.h"
#include "light.h"
#include "group.h"
#include "key.h"

#define TEX_FNAME_BUF_SIZE 200
#define FOO_STRING_BUF_SIZE 200
#define FILE_NAME_SIZE 200

/* This class stores information about the location and direction of a camera
 * in the scene. */

class Camera {
 public:
  Flt heightAngle;
  Flt aspectRatio;
  Point3D position;
  Point3D direction;
  Point3D up;
  Point3D right;

  int read(FILE* fp);
  void write(FILE* fp=stdout);
};

/* This class stores information about a vertex that is used by the 
 * triangles in the scene. */
class Vertex {
 public:
  int index;
  Point3D position;
  Point3D normal;
  Point2D texCoordinate;
  int read(FILE* fp);
  void write(FILE* fp=stdout);
};


/* This class stores information about a material describing shapes in the
 * scene. (This includes a pointer to a Texture if the Shape is to be
 * texture mapped.)*/
class Material {
 public:
  int index;
  Point3D ambient;
  Point3D diffuse;
  Point3D specular;
  Point3D emissive;
  Flt kspec;
  Flt ktran;
  Flt refind;
  GLuint handle;
  class Texture *tex;
  char foo[FOO_STRING_BUF_SIZE];

  Material(void);
  Material(int index,Point3D ambient,Point3D color,Flt kspec);
  void setHandle(void);
  int read(FILE* fp,int& textureIndex);
  void write(FILE* fp=stdout);
};

/* This class stores information about a texture used to texture-map Shapes 
 * in the scene. */

class Texture{
 public:
  int index;
  GLuint handle;
  char filename[TEX_FNAME_BUF_SIZE];
  Image *img;


  Texture(void);
  ~Texture(void);
  void setHandle(void);
  int read(FILE* fp);
  void write(FILE* fp=stdout);
};

/* This class stores information about the scene-graph read out from a .ray
 * file. */

class RayFile{
 public:
  int index;
  char filename[FILE_NAME_SIZE];
  class Scene* scene;

  ~RayFile(void);
  int read(FILE* fp);
  void write(FILE* fp=stdout);
};

/* This class provides access to everything described in the .ray file and
 * stores a copy of the geometry of the scene in the form of a scene-graph. */
class Scene{
	Vertex* vertices;
	int vertexNum;
	Material* materials;
	int materialNum;
	Texture* textures;
	int textureNum;
	RayFile* rayFiles;
	int rayFileNum;
	Parameter** parameters;
	int parameterNum;
	void ParseError(const char *, const char *);
	void ParseLineError(int ,const char*, const char *);
	void ParseGroup(FILE* fp,int& cmndCtr,Group* current,
		const char* fileName);
	void ParseJoint(FILE* fp,int& cmndCtr,Group* current,
		const char* fileName);
	int  ParseShape(FILE* fp,const char* keyword,int cmndCtr,
		Group* current,const char* fileName);
public:
	Point3D ambient;
	Point3D background;
	Camera* camera;
	Light** lights;
	int lightNum;
	KeyFile** keyFiles;
	int keyFileNum;
	StaticGroup* group;

	Scene(void);
	~Scene(void);

	Material* getMaterial(int index);

	void setHandles(void);

	void setParameters(float time);

	void read(const char* fileName);
	void write(FILE* fp=stdout);
};

#endif /* SCENE_INCLUDED */
  

