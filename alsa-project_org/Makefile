all: build build/pcm build/list-devices

clean:
	rm build/*.o

build:
	mkdir -p build

build/pcm.o: pcm.c
	cc -c -o build/pcm.o pcm.c

build/pcm: build/pcm.o
	cc -o build/pcm -lasound -lm build/pcm.o

build/list-devices.o: list-devices.c
	cc -c -o build/list-devices.o list-devices.c

build/list-devices: build/list-devices.o
	cc -o build/list-devices -lasound build/list-devices.o

