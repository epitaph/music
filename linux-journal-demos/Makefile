all: alsa-details alsa-open

alsa-details: alsa-details.o
	gcc -o alsa-details -lasound alsa-details.o

alsa-details.o: alsa-details.c
	gcc -c -o alsa-details.o alsa-details.c

alsa-open: alsa-open.o
	gcc -o alsa-open -lasound alsa-open.o

alsa-open.o: alsa-open.c
	gcc -c -o alsa-open.o alsa-open.c

