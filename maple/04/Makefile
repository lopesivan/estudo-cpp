# Makefile: A standard Makefile for simple.c

CFLAGS= -I/opt/maple2021/extern/include
#LDFLAGS=
LDLIBS=-L/opt/maple2021/bin.X86_64_LINUX -lmaplec -lrt

all  : simple
build:
	LD_LIBRARY_PATH=/opt/maple2021/bin.X86_64_LINUX make
run:
	LD_LIBRARY_PATH=/opt/maple2021/bin.X86_64_LINUX ./simple
clean:
	/bin/rm -rf simple simple.o

# END OF FILE
