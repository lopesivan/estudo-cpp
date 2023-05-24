#!/usr/bin/env bash

ls *_exemplo.cpp |
xargs -L1        |
sort -n          |
tail -1          |
cut -d'_' -f1    |
sed 's/^0//'     |
while read -r n; do 
    printf "cp %02d_exemplo.cpp %02d_exemplo.cpp\n" "$n" "$((n + 1))"; 
    printf "./update-makefile.sh > Makefile\n"; 
done
