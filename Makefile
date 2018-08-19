S=512

CC=gcc
CFLAGS=-Wall -Werror -O3 -std=gnu89 -D SIZE=$(S) -D SIZESTR='"$(S)"'
OBJS=mandelbrot.o pixelColor.o
BIN=server

server : $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS)

bmp : bmpServer.o
	$(CC) $(CFLAGS) -o $(BIN) bmpServer.o

mandelbrot.o : mandelbrot.c pixelColor.h mandelbrot.h tiles.h
pixelColor.o : pixelColor.c pixelColor.h
bmpServer.o : bmpServer.c

clean :
	rm -f $(BIN) $(OBJS) bmpServer.o

.PHONY: clean
