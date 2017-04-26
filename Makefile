CC=gcc
CFLAGS=-Wall -Werror -g -O
OBJS=mandelbrot.o pixelColor.o

all : $(OBJS)
	$(CC) -o server $(OBJS)

mandelbrot.o : mandelbrot.c pixelColor.h mandelbrot.h
pixelColor.o : pixelColor.c pixelColor.h

clean :
	rm -f server $(OBJS)

.PHONY: clean
