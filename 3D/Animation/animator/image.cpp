#include "image.h"
#include "bmp.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

Image::Image(int width, int height){
    assert(width > 0);
    assert(height > 0);

	pixelList=new Pixel[width*height];
	assert(pixelList);
	w=width;
	h=height;
}

Image::~Image(void){delete pixelList;}
int Image::width(void){return w;}
int Image::height(void){return h;}

Pixel& Image::operator()(int x,int y){return pixel(x,y);}
Pixel& Image::pixel(int x, int y){
	assert( x>=0 && x<w);
	assert( y>=0 && y<h);
	return pixelList[y*w+x];
}


