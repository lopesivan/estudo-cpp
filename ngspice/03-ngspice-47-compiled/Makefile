LINUXBREW = $(shell brew --prefix)
NGSPICE   = $(LINUXBREW)/Cellar/ivan/ngspice

CPPFLAGS = `PKG_CONFIG_PATH=$(NGSPICE)/lib/pkgconfig pkg-config --cflags ngspice`
CXXFLAGS = -Wall -Wno-parentheses -std=c++20
LDFLAGS = -Wl,-rpath,$(NGSPICE)/lib
LDLIBS = `PKG_CONFIG_PATH=$(NGSPICE)/lib/pkgconfig pkg-config --libs ngspice`

all: main

run: main
	./main

clean:
	$(RM) main main.o
