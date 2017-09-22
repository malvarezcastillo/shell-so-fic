CC=gcc
CFLAGS=-g -Wall
DEPS=

all: clean p0 p1


p1:
	mkdir -p bin
	gcc -g -Wall -o bin/p1.out src/p1.c

p0:
	mkdir -p bin
	gcc -g -Wall -o bin/p0.out src/p0.c

clean:
	rm -f bin/*.out
