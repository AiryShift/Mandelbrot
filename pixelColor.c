#include "pixelColor.h"

#define RED {255, 0, 0}
#define ORANGE {255, 127, 0}
#define YELLOW {255, 255, 0}
#define GREEN {0, 255, 0}
#define BLUE {0, 0, 255}
#define INDIGO {75, 0, 130}
#define VIOLET {143, 0, 255}

#define MAX_STEPS 256
#define WHITE 255
#define BLACK 0

// Domain of steps: 1 <= steps <= 256
static unsigned char chooseColor(int steps, int type) {
    unsigned char red[] = RED;
    unsigned char orange[] = ORANGE;
    unsigned char yellow[] = YELLOW;
    unsigned char green[] = GREEN;
    unsigned char blue[] = BLUE;
    unsigned char indigo[] = INDIGO;
    unsigned char violet[] = VIOLET;

    int whichColor = (steps - 1) % 7;
    unsigned char outputColor;

    if (whichColor == 0) {
        outputColor = red[type];
    } else if (whichColor == 1) {
        outputColor = orange[type];
    } else if (whichColor == 2) {
        outputColor = yellow[type];
    } else if (whichColor == 3) {
        outputColor = green[type];
    } else if (whichColor == 4) {
        outputColor = blue[type];
    } else if (whichColor == 5) {
        outputColor = indigo[type];
    } else {
        outputColor = violet[type];
    }
    return outputColor;
}

unsigned char stepsToRed(int steps) {
    unsigned char outputColor;
    if (steps == MAX_STEPS) {
        outputColor = WHITE;
    } else if (steps == MAX_STEPS - 1) {
        outputColor = BLACK;
    } else {
        outputColor = chooseColor(steps, 0);
    }
    return outputColor;
}

unsigned char stepsToGreen(int steps) {
    unsigned char outputColor;
    if (steps == MAX_STEPS) {
        outputColor = WHITE;
    } else if (steps == MAX_STEPS - 1) {
        outputColor = BLACK;
    } else {
        outputColor = chooseColor(steps, 1);
    }
    return outputColor;
}

unsigned char stepsToBlue(int steps) {
    unsigned char outputColor;
    if (steps == MAX_STEPS) {
        outputColor = WHITE;
    } else if (steps == MAX_STEPS - 1) {
        outputColor = BLACK;
    } else {
        outputColor = chooseColor(steps, 2);
    }
    return outputColor;
}
