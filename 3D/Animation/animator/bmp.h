#ifndef BMP_INCLUDED
#define BMP_INCLUDED



#include "image.h"



Image *BMPReadImage(FILE *fp);
/* Reads an image from a BMP file. */

void BMPWriteImage(Image *img, FILE *fp);
/* Writes an image to a BMP file. */



#endif /* BMP_INCLUDED */
