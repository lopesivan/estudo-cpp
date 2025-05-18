# Makefile: A standard Makefile for main.cpp

VERSION     := 3.6.0
GLUT_CFLAGS := /home/linuxbrew/.linuxbrew/Cellar/freeglut/$(VERSION)
GLUT_LIBS   := /home/linuxbrew/.linuxbrew/Cellar/freeglut/$(VERSION)

CXXFLAGS = -I$(GLUT_CFLAGS)/include
LDLIBS   = -L$(GLUT_LIBS)/lib -lglut -lGLU -lGL
all  : main
clean:
	/bin/rm -rf main main.o

run:
	LD_LIBRARY_PATH=$(GLUT_LIBS)/lib ./main

# END OF FILE
