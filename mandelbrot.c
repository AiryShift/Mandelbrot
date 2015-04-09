/*
    Insert header comment here
 */

#include <stdio.h>
#include <stdlib.h>
#include "mandelbrot.h"

#define MAX_ITERATIONS 9001 // Placeholder
#define ORIGINAL_VALUE_COUNT 2

int main(int argc, char *argv[]) {
    /* code */
    return EXIT_SUCCESS;
}

int escapeSteps(double x, double y) {
    double originalX = x;
    double originalY = y;
    int iterations = 0;

    while (/* Not infinity */ || iterations < MAX_ITERATIONS) {
        nextTerm(&x, &y, originalX, originalY);
        iterations++;
    }
}

void nextTerm(double *x, double *y, double originalX, double originalY) {
    square(x, y);
    *x += originalX;
    *y += originalY;
}

void square(double *x, double *y) {
    // (x + y)**2 = x**2 + 2xy + y**2
}
