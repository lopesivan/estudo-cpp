#!/usr/bin/env bash

files=(
    "$(find . -name \*.cpp -printf "%P\n"| sort -n)"
)

cat <<EOF
CC = g++
LD = \$(CC)

INCFLAGS =
CPPFLAGS = -std=c++2b -c
LDFLAGS  = -o
GDBFLAGS = -g

target  := `echo ${files//.cpp/}| tr -d '\n'`

targets := \$(target)

# Compile
.cpp.o:
	# compiling: $<
	\$(CC) \$(CPPFLAGS) \$<

all: \$(targets)
$(for f in ${files[*]}; do
    echo ${f%.cpp}: ${f%.cpp}.o
	echo -e "\t# Linking: \$@"
	echo -e "\t\$(CC) \$(LDFLAGS) \$@ \$^"
done)

clean:
	# remove: \$(targets) \$(addsuffix .o, \$(targets))
	/bin/rm -rf \$(targets) \$(addsuffix .o, \$(targets))

# END OF FILE
EOF
