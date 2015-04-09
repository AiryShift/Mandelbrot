/*
    Prints an extremely rough ASCII representation of the Mandelbrot
 */

#include <stdio.h>
#include <stdlib.h>
#include "mandelbrot.c"

#define Y_UPPER_BOUND -1.5
#define Y_LOWER_BOUND (Y_UPPER_BOUND*-1)
#define X_UPPER_BOUND 1.5
#define X_LOWER_BOUND (X_UPPER_BOUND*-1)
#define INCREMENT 0.001
#define NOT_ESCAPED 256

int main(int argc, char *argv[]) {
    double x = X_LOWER_BOUND;
    double y = Y_LOWER_BOUND;
    while (y >= Y_UPPER_BOUND) {
        while (x <= X_UPPER_BOUND) {
            if (escapeSteps(x, y) == NOT_ESCAPED) {
                printf("*");
            } else {
                printf(" ");
            }
            x += INCREMENT;
        }
        printf("\n");
        x = X_LOWER_BOUND;
        y -= INCREMENT;
    }
    return EXIT_SUCCESS;
}
