#include "pixelcolor.h"
#include <math.h>

#define YOLO (256 / 3)

// Domain of steps: 1 <= steps <= 256
unsigned char stepsToRed(int steps) {
    return color(0, steps - 1);
}

unsigned char stepsToGreen(int steps) {
    return color(1, steps - 1);
}

unsigned char stepsToBlue(int steps) {
    return color(2, steps - 1);
}

unsigned char color(int level, int steps) {
    int max[3] = {YOLO * 3, YOLO * 2, YOLO * 0}; 
    int l = max[level];
    
    if (steps >= l) {
        return steps; 
    }

    return 0;
}
