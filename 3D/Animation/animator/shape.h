#ifndef SHAPE_INCLUDED
#define SHAPE_INCLUDED
#include <stdio.h>
#include "geometry.h"

/* 
 * This is the interface that all the objects must implement. 
 * Objects must have accessable matrices to give the transformation to and
 * from world coordinates.
 * Objects must support reading, writing and printing.
 * Objects must support intersection with rays and they must be able to
 * return their bounding boxes.
 */

class Shape{
 public:
  /* These functions are the basic I/O functions for the object */
  virtual char* name(void)=0;
  virtual void write(int indent,FILE* fp=stdout)=0;

  /* This function calls the appropriate openGL code to draw the object. The
   * int specifies the complexity of the tessalation */
  virtual void draw(int complexity)=0;
};
#endif /* SHAPE_INCLUDED */
