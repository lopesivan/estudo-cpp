# Makefile: A standard Makefile for main.c

CPPFLAGS = -I/home/linuxbrew/.linuxbrew/Cellar/libngspice/45/include
LDLIBS   = -L/home/linuxbrew/.linuxbrew/lib -lngspice

all  : main

run: export LD_LIBRARY_PATH=/home/linuxbrew/.linuxbrew/lib
run:
	./main
clean:
	/bin/rm -rf main main.o
# END OF FILE
