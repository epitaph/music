all: build build/pcm

build:
	mkdir -p build

build/pcm.o: pcm.c
	cc -o build/pcm.o pcm.c

build/pcm: build/pcm.o
	cc -o build/pcm -lasound -lm build/pcm.o

