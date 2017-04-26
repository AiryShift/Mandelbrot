CC=gcc
CFLAGS=-Wall -Werror -g -O -std=gnu89
OBJS=mandelbrot.o pixelColor.o

all : $(OBJS)
	$(CC) -o server $(OBJS)

mandelbrot.o : mandelbrot.c pixelColor.h mandelbrot.h
pixelColor.o : pixelColor.c pixelColor.h

clean :
	rm -f server $(OBJS)

.PHONY: clean
