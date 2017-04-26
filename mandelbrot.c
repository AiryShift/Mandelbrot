#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "pixelColor.h"
#include "mandelbrot.h"

// escapeSteps
#define MIN_ITERATIONS 1
#define MAX_ITERATIONS 256
#define ESCAPE_DISTANCE_SQUARED 4

typedef struct _complex {
    double real;
    double imag;
} complex;

typedef struct _coordinate {
    int x;
    int y;
} coordinate;

int isBounded(complex num);
double distanceFromOriginSquared(complex num);
complex nextTerm(complex num, complex initial);
complex complexSquare(complex num);
double square(double x);
double twoToThePowerOf(int exponent);

// Server
#define SIMPLE_SERVER_VERSION 1.0
#define REQUEST_BUFFER_SIZE 1000
#define DEFAULT_PORT 1917
#define SIZEOF_URL 1024

void serveHTML(int socket);
void serveBMP(int socket, complex imageCenter, int zoom);
int makeServerSocket(int portno);
int waitForConnection(int serverSocket);

// BMP
#define BYTES_PER_PIXEL 3
#define BITS_PER_PIXEL (BYTES_PER_PIXEL*8)
#define NUMBER_PLANES 1
#define PIX_PER_METRE 2835
#define MAGIC_NUMBER 0x4d42
#define NO_COMPRESSION 0
#define OFFSET 54
#define DIB_HEADER_SIZE 40
#define NUM_COLORS 0

#define SIZE 512
#define TOTAL_NUM_BYTES (SIZE*SIZE*BYTES_PER_PIXEL)

void writeHeader(int socket);
complex findPixelCenter(complex imageCenter, coordinate curPos, double zoom);
void writePixel(unsigned char bmp[], int stepsTaken, unsigned long address);

int main(int argc, char *argv[]) {
    printf("************************************\n");
    printf("Starting mandelbrot server %f\n", SIMPLE_SERVER_VERSION);
    printf("Serving mandelbrot since 2015\n");

    int serverSocket = makeServerSocket(DEFAULT_PORT);
    printf("Access this server at http://localhost:%d/\n", DEFAULT_PORT);
    printf("************************************\n");

    char request[REQUEST_BUFFER_SIZE];

    while (1) {
        int connectionSocket = waitForConnection(serverSocket);
        // wait for a request to be sent from a web browser, open a new
        // connection for this conversation

        // read the first line of the request sent by the browser
        int bytesRead;
        bytesRead = read(connectionSocket, request, (sizeof request) - 1);
        assert(bytesRead >= 0);
        // were we able to read any data from the connection?

        // print entire request to the console
        printf("*** Received http request ***\n %s\n", request);

        //send the browser a simple html page using http
        printf("*** Sending http response ***\n");

        // harrison's magical URL grepper code
        char url[SIZEOF_URL];
        sscanf(request, "GET %s HTTP/1", url);
        printf("\n%s\n", url);

        // print entire request to the console
        printf("*** Received http request ***\n%s\n", request);

        if (strcmp(url, "/") == 0) {
            printf("Client is requesting the home page\n");
            serveHTML(connectionSocket);
        } else {
            printf("Client is requesting a tile (%s)\n", url);
            double x, y;
            int z;

            sscanf(url, "/tile_x%lf_y%lf_z%d.bmp", &x, &y, &z);

            printf("x: %lf, y: %lf, zoom: %d\n\n", x, y, z);

            // send the browser a simple html page using http
            printf("*** Sending http response ***\n");
            complex center = {x, y};
            serveBMP(connectionSocket, center, z);
        }

        // close the connection after sending the page
        close(connectionSocket);
    }

    // close the server connection after we are done
    printf("** shutting down the server **\n");
    close(serverSocket);

    return EXIT_SUCCESS;
}

int escapeSteps(double x, double y) {
    complex initialValue = {x, y};
    complex workingCopy = initialValue;
    int iterations = MIN_ITERATIONS;

    while (isBounded(workingCopy) && iterations < MAX_ITERATIONS) {
        workingCopy = nextTerm(workingCopy, initialValue);
        iterations++;
    }
    return iterations;
}

int isBounded(complex num) {
    return (distanceFromOriginSquared(num) < ESCAPE_DISTANCE_SQUARED);
}

double distanceFromOriginSquared(complex num) {
    return square(num.real) + square(num.imag);
}

complex nextTerm(complex num, complex initial) {
    num = complexSquare(num);
    num.real += initial.real;
    num.imag += initial.imag;
    return num;
}

complex complexSquare(complex num) {
    // The square of a complex number resolves to:
    // (x + yi)**2 = (x**2 - y**2) + (2xy)i
    complex temp = num;
    num.real = square(temp.real) - square(temp.imag);
    num.imag = 2 * temp.real * temp.imag;
    return num;
}

double twoToThePowerOf(int exponent) {
    int i = 0;
    double result = 1;
    while (i < exponent) {
        result *= 2;
        i++;
    }
    return result;
}

double square(double x) {
    return x * x;
}

void serveHTML(int socket) {
    char* message;

    // first send the http response header
    message =
        "HTTP/1.0 200 Found\r\n"
        "Content-Type: text/html\r\n"
        "\r\n";
    printf("about to send=> %s\n", message);
    write(socket, message, strlen(message));

    message =
        "<!DOCTYPE html>\n"
        "<script src=\"http://almondbread.cse.unsw.edu.au/tiles.js\">"
        "</script>"
        "\n";
    printf("now sending=> %s\n", message);
    write(socket, message, strlen(message));
}

void serveBMP(int socket, complex imageCenter, int zoom) {
    char* message;

    // first send the http response header
    message = "HTTP/1.0 200 OK\r\n"
              "Content-Type: image/bmp\r\n"
              "\r\n";
    printf("about to send=> %s\n", message);
    write(socket, message, strlen(message));

    // Send BMP header
    writeHeader(socket);

    // Send BMP content
    unsigned char bmp[TOTAL_NUM_BYTES];
    unsigned long bytesWritten = 0;
    coordinate curPos = {0, 0};
    double distanceBetweenPixels = 1 / twoToThePowerOf(zoom);
    printf("distanceBetweenPixels: %lf\n", distanceBetweenPixels);

    while (bytesWritten < TOTAL_NUM_BYTES) {
        complex pixelCenter = findPixelCenter(imageCenter, curPos, distanceBetweenPixels);
        int stepsTaken = escapeSteps(pixelCenter.real, pixelCenter.imag);
        writePixel(bmp, stepsTaken, bytesWritten);

        bytesWritten += BYTES_PER_PIXEL;
        curPos.x++;
        if (curPos.x == SIZE) {
            curPos.x = 0;
            curPos.y++;
        }
    }
    write(socket, bmp, sizeof(bmp));
}

complex findPixelCenter(complex imageCenter, coordinate curPos, double zoom) {
    curPos.x -= SIZE / 2;
    curPos.y -= SIZE / 2;

    complex center;
    center.real = imageCenter.real + (curPos.x - 0.5) * zoom;
    center.imag = imageCenter.imag + (curPos.y - 0.5) * zoom;
    return center;
}

void writePixel(unsigned char bmp[], int stepsTaken, unsigned long address) {
    bmp[address] = stepsToBlue(stepsTaken);
    bmp[address + 1] = stepsToGreen(stepsTaken);
    bmp[address + 2] = stepsToRed(stepsTaken);
}

// start the server listening on the specified port number
int makeServerSocket(int portNumber) {
    // create socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    assert(serverSocket >= 0); // error opening socket

    // bind socket to listening port
    struct sockaddr_in serverAddress;
    bzero((char *) &serverAddress, sizeof(serverAddress));

    serverAddress.sin_family      = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port        = htons(portNumber);

    // let the server start immediately after a previous shutdown
    int optionValue = 1;
    setsockopt(
        serverSocket,
        SOL_SOCKET,
        SO_REUSEADDR,
        &optionValue,
        sizeof(int)
    );

    int bindSuccess =
        bind(
            serverSocket,
            (struct sockaddr *) &serverAddress,
            sizeof (serverAddress)
        );

    assert(bindSuccess >= 0);
    // if this assert fails wait a short while to let the operating
    // system clear the port before trying again

    return serverSocket;
}

// wait for a browser to request a connection,
// returns the socket on which the conversation will take place
int waitForConnection(int serverSocket) {
    // listen for a connection
    const int serverMaxBacklog = 10;
    listen(serverSocket, serverMaxBacklog);

    // accept the connection
    struct sockaddr_in clientAddress;
    socklen_t clientLen = sizeof(clientAddress);
    int connectionSocket =
        accept(
            serverSocket,
            (struct sockaddr *) &clientAddress,
            &clientLen
        );

    assert(connectionSocket >= 0); // error on accept

    return(connectionSocket);
}

void writeHeader(int socket) {
    unsigned short magicNumber = MAGIC_NUMBER;
    write(socket, &magicNumber, sizeof(magicNumber));

    unsigned int fileSize = OFFSET + (SIZE * SIZE * BYTES_PER_PIXEL);
    write(socket, &fileSize, sizeof(fileSize));

    unsigned int reserved = 0;
    write(socket, &reserved, sizeof(reserved));

    unsigned int offset = OFFSET;
    write(socket, &offset, sizeof(offset));

    unsigned int dibHeaderSize = DIB_HEADER_SIZE;
    write(socket, &dibHeaderSize, sizeof(dibHeaderSize));

    unsigned int width = SIZE;
    write(socket, &width, sizeof(width));

    unsigned int height = SIZE;
    write(socket, &height, sizeof(height));

    unsigned short planes = NUMBER_PLANES;
    write(socket, &planes, sizeof(planes));

    unsigned short bitsPerPixel = BITS_PER_PIXEL;
    write(socket, &bitsPerPixel, sizeof(bitsPerPixel));

    unsigned int compression = NO_COMPRESSION;
    write(socket, &compression, sizeof(compression));

    unsigned int imageSize = (SIZE * SIZE * BYTES_PER_PIXEL);
    write(socket, &imageSize, sizeof(imageSize));

    unsigned int hResolution = PIX_PER_METRE;
    write(socket, &hResolution, sizeof(hResolution));

    unsigned int vResolution = PIX_PER_METRE;
    write(socket, &vResolution, sizeof(vResolution));

    unsigned int numColors = NUM_COLORS;
    write(socket, &numColors, sizeof(numColors));

    unsigned int importantColors = NUM_COLORS;
    write(socket, &importantColors, sizeof(importantColors));
}
