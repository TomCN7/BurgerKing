#ifndef IMAGE_INCLUDED
#define IMAGE_INCLUDED

#include <stdio.h>

typedef unsigned char Component;


typedef struct {
    Component r, g, b, a;
} Pixel;

class Image{
	int w,h;
public:
	Pixel* pixelList;

	Image(int width,int height);
	~Image(void);

	int width(void);
	int height(void);
	Pixel& operator()(int x,int y);
	Pixel& pixel(int x, int y);
};
#endif /* IMAGE_INCLUDED */

