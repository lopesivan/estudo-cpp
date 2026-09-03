LUAENV                 := $(shell luaenv prefix 5.4.6)
PKG_CONFIG_SYSTEM_PATH := /usr/lib/x86_64-linux-gnu/pkgconfig

LUA_CFLAGS     := $(shell PKG_CONFIG_PATH=$(LUAENV)/pkgconfig pkg-config --cflags lua54)
LUA_LIBS       := $(shell PKG_CONFIG_PATH=$(LUAENV)/pkgconfig pkg-config --libs lua54)

CXX = g++

CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++20 -O2

NGSPICE = $(shell brew --prefix)/Cellar/ivan/ngspice

CPPFLAGS = \
	-I$(NGSPICE)/include \
	$(LUA_CFLAGS)

LDFLAGS = \
	-Wl,-rpath,$(NGSPICE)/lib

LDLIBS = \
	-L$(NGSPICE)/lib \
	-lngspice \
	$(LUA_LIBS)

all: main

main: main.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $< $(LDLIBS) -o $@

run: main
	./main

clean:
	$(RM) main

.PHONY: all run clean
