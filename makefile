all:
	gcc -Wall -Werror -O -o server bmpServer.c
	./server

test:
	gcc -Wall -Werror -O -o server mandelbrotTest.c
