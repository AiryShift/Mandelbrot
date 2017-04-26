# Mandelbrot
__HS1917 2nd Assignment__

## Pixel Colours

You may choose how to colour the pixels yourself, using your artistic, analytical and visual design skills.  Some suggestions are given below, there are many ideas on the internet, and we'll discuss this more in lectures also.
The basic idea is to colour each pixel based on the number of iterations the above process takes.
The simplest approach is to colour a pixel black if you took 256 iterations, white otherwise.  This will give you a black and white image.
A more interesting way of colouring the pixels is to shade them grey - with intensity depending on the number of iterations.
The most powerful and effective way of colouring the pixels is to use a different rgb colour for each possible number of iterations.  That is how the images on this page were generated.  The artistry here lies in picking the colours and deciding how they correspond to the different iteration counts.

Put your color decision code into 
`pixelColor.c`

put the coloring-in logic into the 3 functions we discussed in lectures (their prototypes are in `pixelColor.h`)  
If you put any other helper functions into `pixelColor.c` you must make them static.  (put the word "static" at the start of the functions' declaration and definition).
Submit your best images (and the corresponding `pixelColor.c` files) to the Snapshots competition (Activity released in Module 6).

## Files
Put all your mandelbrot and server code including the main function into a file called `mandelbrot.c`, and put your color related functions into a file called `pixelColor.c`.  Both of these .c files should #include `pixelColor.h`, and `mandelbrot.c` should also `#include mandelbrot.h`.  

Implement the function prototypes from the .h files in the correspondingly named .c files.  Make any additional helper functions you write static. Don't change the .h files, we'll use our own copy of the .h files when marking - you'll only submit the two .c files.

## Running
Compile with `make`. Alternatively, try `gcc -Wall -Werror -O -std=gnu89 -o server mandelbrot.c pixelColor.c`. Run with `./server`.
