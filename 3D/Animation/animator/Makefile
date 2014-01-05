CC=gcc
INCS=-I/usr/include/GL
LIBS=-L/usr/X11R6/lib -L/usr/X11/lib/GL -lm -lglut -lX11 -lGL -lGLU

SRCS=bmp.cpp box.cpp cone.cpp cylinder.cpp directionalLight.cpp geometry.cpp group.cpp image.cpp key.cpp main.cpp pointLight.cpp rayFileInstance.cpp rayView.cpp scene.cpp sphere.cpp spotLight.cpp pMatrix.cpp triangle.cpp
OBJS=$(SRCS:.cpp=.o)

CCFLAGS=-DUSE_UNIX -g -Wall

.SUFFIXES: .cpp

viewer: $(OBJS)
	$(CC) -o animator $(INCS) $(CCFLAGS) $(OBJS) $(LIBS)

.cpp.o:
	$(CC) $(CCFLAGS) -c $(INCS) $<

clean:
	-rm -f core
	-rm -f *.o
	-rm -f *~
	-rm -f animator

