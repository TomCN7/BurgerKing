#ifndef GEOMETRY_INCLUDED
#define GEOMETRY_INCLUDED
#include <math.h>
#include <stdio.h>
#include <assert.h>

#define ASSERT_ARRAY 0
#define RAYEPS 1e-5

/* *LEAVE* this as double!*/
typedef double Flt;


/* The Point2D and Point3D classes are just a 2D and 3D vectors with methods 
 * supporting indexing, printing, taking the dot product with another vector, 
 * obtaining length, returning a unit-length scaled version of itself,
 * negation, scaling, adding, subtracting, and doing component-wise 
 * multiplication. Additionally, the 3D vector supports taking the cross-
 * product. */

/* Note that coordinates of the vector are accessed using square brackets in 
 * the same way that an array is so that:
 *   Point3D p(0.0,1.0,2.0);
 *   printf("%f\n",p[1]);
 * should print out "1.0" */

class Point2D{
  Flt p[2];
 public:
  Point2D(){p[0]=p[1]=0.0;}
  Point2D(Flt x,Flt y){p[0]=x;p[1]=y;}

  Flt& operator[] (int index);

  void print(){printf("(%f,%f)",p[0],p[1]);}
  void printnl(){print();printf("\n");}

  Flt dot(Point2D pt);

  Flt length(void);

  Point2D unit(void);

  Point2D negate(void);
  Point2D operator- (void);

  Point2D scale(Flt scl);
  Point2D operator* (Flt scale);
  Point2D operator/ (Flt scale);

  Point2D add(Point2D pt);
  Point2D operator+ (Point2D pt);

  Point2D subtract(Point2D pt);
  Point2D operator- (Point2D pt);

  Point2D mult(Point2D pt);
};

class Point3D{
  Flt p[3];
 public:
  Point3D(){p[0]=p[1]=p[2]=0;}
  Point3D(Flt x,Flt y,Flt z){p[0]=x;p[1]=y;p[2]=z;}

  Flt& operator[] (int index);

  void print(void){printf("(%f,%f,%f)",p[0],p[1],p[2]);}
  void printnl(void){print();printf("\n");}

  Flt dot(Point3D pt);

  Flt length(void);

  Point3D unit(void);

  Point3D negate(void);
  Point3D operator- (void);

  Point3D scale(Flt);
  Point3D operator* (Flt scale);
  Point3D operator/ (Flt scale);

  Point3D add(Point3D pt);
  Point3D operator+ (Point3D pt);

  Point3D subtract(Point3D pt);
  Point3D operator- (Point3D pt);

  Point3D crossProduct(Point3D pt);

  Point3D mult(Point3D pt);
};

/* A ray is a directed line segment which is defined by a pair of 3D
 * coordinates (or points). The first point specifies the starting point of the
 * ray and the second specifies its direction. It supports methods for
 * translating, finding the paramtetrized distance along a ray, and taking
 * the dot product. */

/* In terms of the position operators are equivalent to:
 *  r(s)=r.position(s)=r.p+r.q*s;
 *  r.dot(m)=(r.q-r.p).dot(m)
 * where "s" is floating point number and "m" is Point3D. (You don't need them
 * but they may help.) */

class Ray{
 public:
  Point3D p;
  Point3D d;

  Ray(){;}
  Ray(Point3D p1,Point3D d1){p=p1;d=d1;}

  void print(void){p.print();printf(":");d.print();}
  void printnl(void){print();printf("\n");}

  Ray translate(Point3D pt);

  Point3D operator() (Flt param);
  Point3D position(Flt param);
};

/* The Matrix class is a 4x4 matrix and supports: taking of determinant,
 * multiplying by another matrix on the right, transposing, inverting,
 * transforming a position, transforming a direction, transforming a normal,
 * and transforming a ray */

/* Its elements are indexed using parentheses and are indexed first by column
 * and the by row. Thus:
 *    Matrix m;
 *    float  f;
 *    ...
 *    f=m(i,j);
 * Sets "f" equal to the value in the matrix stored in column "i" and row "j".
 * (Indexing starts at zero.) */

/* Note that since the matrix is actually a 4x4 matrix it is also used to
 * represent translations. For this reason it supports multiplication by the
 * three different kind of points:
 *  1) Multiplying a positional point performs the full transformation
 *  2) Multiplying a direction does not perform the translation, and
 *  3) Multiplying a normal returns a unit-vector which corresponds to the
 *     transformed normal. (i.e. If "n" is a vector normal to a plane then
 *     Matrix.multNormal(n) returns a unit-vector that is normal to the 
 *     transformed plane.) */

/* Indexed via Matrix(col,row)*/
class Matrix{
  Flt subDet(int col1,int row1,int col2,int row2);
  Flt subDet(int col,int row);
  Flt m[4][4];
 public:
  Flt& operator() (int col,int row);

  Flt det(void);

  void print(void);
  void printnl(void){print();printf("\n");}

  Matrix mult(Matrix m);
  Matrix operator* (Matrix m);

  Matrix transpose(void);

  Matrix invert(void);

  Point3D multPosition(Point3D position);
  Point3D multDirection(Point3D direction);
  Point3D multNormal(Point3D normal);

  Ray mult(Ray ray);
  Ray operator* (Ray ray);

  int isIdentity(void);
};
Matrix IdentityMatrix(void);
#endif /* GEOMETRY_INCLUDED */
