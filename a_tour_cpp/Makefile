CC = g++
LD = $(CC)

INCFLAGS =
CPPFLAGS = -std=c++2b -c
LDFLAGS  = -o
GDBFLAGS = -g

target  := 01_exemplo 02_exemplo 03_exemplo 04_exemplo 05_exemplo 06_exemplo 07_exemplo

targets := $(target)

# Compile
.cpp.o:
	# compiling: $<
	$(CC) $(CPPFLAGS) $<

all: $(targets)
01_exemplo: 01_exemplo.o
	# Linking: $@
	$(CC) $(LDFLAGS) $@ $^
02_exemplo: 02_exemplo.o
	# Linking: $@
	$(CC) $(LDFLAGS) $@ $^
03_exemplo: 03_exemplo.o
	# Linking: $@
	$(CC) $(LDFLAGS) $@ $^
04_exemplo: 04_exemplo.o
	# Linking: $@
	$(CC) $(LDFLAGS) $@ $^
05_exemplo: 05_exemplo.o
	# Linking: $@
	$(CC) $(LDFLAGS) $@ $^
06_exemplo: 06_exemplo.o
	# Linking: $@
	$(CC) $(LDFLAGS) $@ $^
07_exemplo: 07_exemplo.o
	# Linking: $@
	$(CC) $(LDFLAGS) $@ $^

clean:
	# remove: $(targets) $(addsuffix .o, $(targets))
	/bin/rm -rf $(targets) $(addsuffix .o, $(targets))

# END OF FILE
