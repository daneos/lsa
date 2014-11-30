#!/bin/bash
gcc -c -o options.o options.c
gcc -c -o fmode.o fmode.c
gcc -c -o print_entry.o print_entry.c
gcc -c -o perm_filters.o perm_filters.c
gcc -c -o lsa.o lsa.c

gcc -o lsa fmode.o perm_filters.o options.o print_entry.o lsa.o -lm