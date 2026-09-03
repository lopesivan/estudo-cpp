NGSPICE := /home/linuxbrew/.linuxbrew/Cellar/ivan/ngspice/bin/ngspice
NETLIST := RC.sp
DATA    := rc.dat measures.dat
PLOT    := plot.py
PYTHON  := PYENV_VERSION=jupyter python3

.PHONY: all run plot clean

all: plot

run:
	$(NGSPICE) -b $(NETLIST)

plot: run
	$(PYTHON) $(PLOT)

clean:
	rm -f $(DATA)
