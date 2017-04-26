all:
	gcc -Wall -Werror -O -o server mandelbrot.c pixelColor.c
	./server

test:
	gcc -Wall -Werror -O -o server mandelbrotTest.c
