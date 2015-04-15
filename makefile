all:
	gcc -Wall -Werror -O -o server mandelbrot.c
	./server

test:
	gcc -Wall -Werror -O -o server mandelbrotTest.c
