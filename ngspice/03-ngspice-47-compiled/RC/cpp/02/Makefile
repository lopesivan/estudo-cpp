NGSPICE  := /home/linuxbrew/.linuxbrew/Cellar/ivan/ngspice/bin/ngspice
NETLIST  := RC.sp
DATA     := rc.dat

CPPFLAGS  =
CXXFLAGS  = -Wall -Wno-parentheses -std=c++20
LDFLAGS   =
LDLIBS    =

.PHONY: all run clean

all: main

$(DATA):
	$(NGSPICE) -b $(NETLIST)

run: main $(DATA)
	./main

clean:
	$(RM) $(DATA)
	$(RM) main main.o
