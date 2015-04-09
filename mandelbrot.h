/*
 *  mandelbrot.h
 *  mandelbrot server
 *
 *  Created by Richard Buckland on 13/04/13.
 *  Licensed under Creative Commons BY 3.0.
 *
 */

int escapeSteps(double x, double y);
int isBounded(double x, double y);
void nextTerm(double *x, double *y, double originalX, double originalY);
void complexSquare(double *x, double *y);
double square(double x);
double root(double x);
