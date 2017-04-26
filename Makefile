CC=gcc
CFLAGS=-Wall -Werror -g -O -std=gnu89
OBJS=mandelbrot.o pixelColor.o
BIN=server

all : $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS)

bmp : bmpServer.o
	$(CC) $(CFLAGS) -o $(BIN) bmpServer.o

mandelbrot.o : mandelbrot.c pixelColor.h mandelbrot.h
pixelColor.o : pixelColor.c pixelColor.h
bmpServer.o : bmpServer.c

clean :
	rm -f $(BIN) $(OBJS) bmpServer.o

.PHONY: clean
