LINUXBREW := $(shell brew --prefix)
NGSPICE   := $(LINUXBREW)/Cellar/ivan/ngspice

PKG_CONFIG_PATH := $(NGSPICE)/lib/pkgconfig

CPPFLAGS := $(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --cflags ngspice)
CXXFLAGS := -Wall -Wextra -Wpedantic -std=c++20 -O2
LDFLAGS  := -Wl,-rpath,$(NGSPICE)/lib
LDLIBS   := $(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs ngspice)

.PHONY: all run clean

all: main

run: main
	./main

clean:
	$(RM) main main.o
