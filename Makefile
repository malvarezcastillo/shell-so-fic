CC=gcc
CFLAGS=-g -Wall
DEPS=

all: p0

p0:
	mkdir -p bin
	gcc -g -Wall -o bin/p0 src/p0.c

clean:
	rm -f bin/p0
