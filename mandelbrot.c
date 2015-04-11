#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mandelbrot.h"

#define MIN_ITERATIONS 1
#define MAX_ITERATIONS 256
#define ESCAPE_DISTANCE 2

static int isBounded(double x, double y);
static void nextTerm(double *x, double *y, double initX, double initY);
static void complexSquare(double *x, double *y);
static double square(double x);

int escapeSteps(double x, double y) {
    double initX = x;
    double initY = y;
    int iterations = MIN_ITERATIONS; // Mandelbrot assumes this

    while (isBounded(x, y) && iterations < MAX_ITERATIONS) {
        nextTerm(&x, &y, initX, initY);
        iterations++;
    }
    return iterations;
}

static int isBounded(double x, double y) {
    int bounded = 1;
    if (sqrt(square(x) + square(y)) >= ESCAPE_DISTANCE) { // Distance f
        bounded = 0;
    }
    return bounded;
}

static void nextTerm(double *x, double *y, double initX, double initY) {
    complexSquare(x, y);
    *x += initX;
    *y += initY;
}

static void complexSquare(double *x, double *y) {
    // The square of a complex number resolves to:
    // (x + yi)**2 = (x**2 - y**2) + (2xy)i
    double tempX = *x;
    double tempY = *y;
    *x = square(tempX) - square(tempY);
    *y = 2 * tempX * tempY;
}

static double square(double x) {
    return pow(x, 2);
}
