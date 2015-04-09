/*
    Insert header comment here
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mandelbrot.h"

#define MAX_ITERATIONS 256

/*
int main(int argc, char *argv[]) {
    return EXIT_SUCCESS;
}
*/

int escapeSteps(double x, double y) {
    double firstX = x;
    double firstY = y;
    int iterations = 1; // Mandelbrot assumes minimum 1 iteration

    while (isBounded(x, y) && iterations < MAX_ITERATIONS) {
        nextTerm(&x, &y, firstX, firstY);
        iterations++;
    }
    return iterations;
}

int isBounded(double x, double y) {
    int bounded = 1;
    if (root(square(x) + square(y)) >= 2) { // Distance formula
        bounded = 0;
    }
    return bounded;
}

void nextTerm(double *x, double *y, double firstX, double firstY) {
    complexSquare(x, y);
    *x += firstX;
    *y += firstY;
}

void complexSquare(double *x, double *y) {
    // (x + yi)**2 = (x**2 - y**2) + 2xyi
    double tempX = *x;
    double tempY = *y;
    *x = square(tempX) - square(tempY);
    *y = 2 * tempX * tempY;
}

double square(double x) {
    return pow(x, 2);
}

double root(double x) {
    return pow(x, 0.5);
}
