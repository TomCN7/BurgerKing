#include "geometry.h"

/* These are the implementations of the promised methods for Point2D */
Flt& Point2D::operator[] (int i){
  if(ASSERT_ARRAY){assert(0<=i && i<2);}
  return p[i];
}
Point2D Point2D::unit(void){
  Flt l=length();
  assert(l!=0.0);
  return scale(1.0/l);
}
Point2D Point2D::negate(void){return scale(-1.0);}
Point2D Point2D::operator -(void){return scale(-1.0);}

Point2D Point2D::scale(Flt s){return Point2D(p[0]*s,p[1]*s);}
Point2D Point2D::operator* (Flt s){return scale(s);}
Point2D Point2D::operator/ (Flt s){return scale(1.0/s);}

Flt     Point2D::dot(Point2D q){return p[0]*q.p[0]+p[1]*q.p[1];}
Flt     Point2D::length(void){return sqrt(dot(*this));}

Point2D Point2D::add(Point2D q){
  return Point2D(p[0]+q.p[0],p[1]+q.p[1]);
}
Point2D Point2D::operator+ (Point2D q){return add(q);}

Point2D Point2D::subtract(Point2D q){
  return Point2D(p[0]-q.p[0],p[1]-q.p[1]);
}
Point2D Point2D::operator- (Point2D q){return subtract(q);}

Point2D Point2D::mult(Point2D q){
  return Point2D(p[0]*q[0],p[1]*q[1]);
}


/* These are the implementations of the promised methods for Point3D */
Flt&    Point3D::operator[] (int i){
  if(ASSERT_ARRAY){assert(0<=i && i<3);}
  return p[i];
}

Point3D Point3D::unit(void){
  Flt l=length();
  assert(l!=0.0);
  return scale(1.0/l);
}
Point3D Point3D::negate(void){return scale(-1.0);}
Point3D Point3D::operator -(void){return scale(-1.0);}

Point3D Point3D::scale(Flt s){return Point3D(p[0]*s,p[1]*s,p[2]*s);}
Point3D Point3D::operator* (Flt s){return scale(s);}
Point3D Point3D::operator/ (Flt s){return scale(1.0/s);}

Flt     Point3D::dot(Point3D q){return p[0]*q.p[0]+p[1]*q.p[1]+p[2]*q.p[2];}
Flt     Point3D::length(void){return sqrt(dot(*this));}

Point3D Point3D::add(Point3D q){
  return Point3D(p[0]+q.p[0],p[1]+q.p[1],p[2]+q.p[2]);
}
Point3D Point3D::operator+ (Point3D q){return add(q);}

Point3D Point3D::subtract(Point3D q){
  return Point3D(p[0]-q.p[0],p[1]-q.p[1],p[2]-q.p[2]);
}
Point3D Point3D::operator- (Point3D q){return subtract(q);}

Point3D Point3D::crossProduct(Point3D q){
  return Point3D(p[1]*q[2]-p[2]*q[1],-p[0]*q[2]+p[2]*q[0],p[0]*q[1]-p[1]*q[0]);
}
Point3D Point3D::mult(Point3D q){
  return Point3D(p[0]*q[0],p[1]*q[1],p[2]*q[2]);
}

/* These are the implementations of the promised methods for Ray */
Ray Ray::translate(Point3D q){return Ray(p+q,d);}
Point3D Ray::position(Flt s){
  return Point3D(p[0]+s*d[0],p[1]+s*d[1],p[2]+s*d[2]);
}
Point3D Ray::operator() (Flt s){return position(s);}


/* These are the implementations of the promised methods for Matrix */
Flt& Matrix::operator() (int i,int j){
  if(ASSERT_ARRAY){assert(0<=i && 0<=j && i<4 && j<4);}
  return m[i][j];
}
Flt  Matrix::det(void){
  Flt d=0.0;
  int i;
  for(i=0;i<4;i++){
    if((i%2)==0){d+=subDet(i,0)*this->m[i][0];}
    else{d-=subDet(i,0)*this->m[i][0];}
  }
  return d;
}
Flt Matrix::subDet(int c1,int r1,int c2,int r2){
  return this->m[c1][r1]*this->m[c2][r2]-this->m[c1][r2]*this->m[c2][r1];
}
Flt Matrix::subDet(int c,int r){
  int i;
  int c1,r1,c2,r2,row;
  Flt d=0,sgn=1.0;
  row=0;
  if(row==r){row++;}
  for(i=0;i<4;i++){
    if(i==c){continue;}
    c1=0;
    while(c1==i || c1==c){c1++;}
    c2=c1+1;
    while(c2==i || c2==c){c2++;}
    r1=0;
    while(r1==row || r1==r){r1++;}
    r2=r1+1;
    while(r2==row || r2==r){r2++;}

    d+=sgn*this->m[i][row]*subDet(c1,r1,c2,r2);
    sgn*=-1.0;
  }
  return d;
}

void Matrix::print(void){
  int i,j;
  for(j=0;j<4;j++){
    for(i=0;i<4;i++){printf("%5.3f ",this->m[i][j]);}
    printf("\n");
  }
}


Matrix Matrix::mult(Matrix m){
  int i,j,k;
  Matrix n;
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      n(i,j)=0.0;
      for(k=0;k<4;k++){n(i,j)+=this->m[k][j]*m(i,k);}
    }
  }
  return n;
}
Matrix Matrix::operator* (Matrix m){return mult(m);}
Matrix Matrix::transpose(void){
  int i,j;
  Matrix n;
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){n(i,j)=this->m[j][i];}
  }
  return n;
}
Matrix Matrix::invert(void){
  int i,j;
  Matrix m;
  Flt d;
  
  d=det();
  assert(d!=0.0);
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      if((i+j)%2==0){m(j,i)=subDet(i,j)/d;}
      else{m(i,j)=-subDet(j,i)/d;}
    }
  }
  return m;
}
Point3D Matrix::multPosition(Point3D p){
  Point3D q;
  int i,j;
  for(i=0;i<3;i++){
    q[i]=this->m[3][i];
    for(j=0;j<3;j++){q[i]+=this->m[j][i]*p[j];}
  }
  return q;
}
Point3D Matrix::multDirection(Point3D p){
  Point3D q;
  int i,j;
  for(i=0;i<3;i++){
    q[i]=0.0;
    for(j=0;j<3;j++){q[i]+=this->m[j][i]*p[j];}
  }
  return q;
}
Point3D Matrix::multNormal(Point3D p){
  return transpose().invert().multDirection(p);
}

Ray Matrix::mult(Ray r){
  Ray q;
  q.p=multPosition(r.p);
  q.d=multDirection(r.d);
  return q;
}
Ray Matrix::operator* (Ray r){return mult(r);}
int Matrix::isIdentity(void){
  int i,j;
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      if(i==j && m[i][j]!=1.0){return 0;}
      else if(i!=j && m[i][j]!=0.0){return 0;}
    }
  }
  return 1;
}

Flt det(Matrix m){return m.det();}
Matrix IdentityMatrix(void){
  Matrix m;
  int i,j;
  for(i=0;i<4;i++){for(j=0;j<4;j++){m(i,j)=(i==j)?1.0:0.0;}}
  return m;
}
