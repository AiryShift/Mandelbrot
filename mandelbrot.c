#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include "mandelbrot.h"

// escapeSteps
#define MIN_ITERATIONS 1
#define MAX_ITERATIONS 256
#define ESCAPE_DISTANCE 2

static int isBounded(double x, double y);
static void nextTerm(double *x, double *y, double initX, double initY);
static void complexSquare(double *x, double *y);
static double square(double x);

// Server
#define SIMPLE_SERVER_VERSION 1.0
#define REQUEST_BUFFER_SIZE 1000
#define DEFAULT_PORT 1917
#define NUMBER_OF_PAGES_TO_SERVE 10
// after serving this many pages the server will halt

void serveBMP(int socket);
int makeServerSocket(int portno);
int waitForConnection(int serverSocket);

int main(int argc, char *argv[]) {
    printf ("************************************\n");
    printf ("Starting simple server %f\n", SIMPLE_SERVER_VERSION);
    printf ("Serving bmps since 2012\n");

    int serverSocket = makeServerSocket (DEFAULT_PORT);
    printf ("Access this server at http://localhost:%d/\n", DEFAULT_PORT);
    printf ("************************************\n");

    char request[REQUEST_BUFFER_SIZE];

    int numberServed = 0;
    while (numberServed < NUMBER_OF_PAGES_TO_SERVE) {
        printf ("*** So far served %d pages ***\n", numberServed);

        int connectionSocket = waitForConnection (serverSocket);
        // wait for a request to be sent from a web browser, open a new
        // connection for this conversation

        // read the first line of the request sent by the browser
        int bytesRead;
        bytesRead = read (connectionSocket, request, (sizeof request)-1);
        assert (bytesRead >= 0);
        // were we able to read any data from the connection?

        // print entire request to the console
        printf (" *** Received http request ***\n %s\n", request);

        //send the browser a simple html page using http
        printf (" *** Sending http response ***\n");
        serveBMP(connectionSocket);

        // close the connection after sending the page- keep aust beautiful
        close(connectionSocket);

        numberServed++;
    }

    // close the server connection after we are done- keep aust beautiful
    printf ("** shutting down the server **\n");
    close (serverSocket);

    return EXIT_SUCCESS;
}

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


void serveBMP (int socket) {
    char* message;

    // first send the http response header
    message = "HTTP/1.0 200 OK\r\n"
              "Content-Type: image/bmp\r\n"
              "\r\n";
    printf ("about to send=> %s\n", message);
    write (socket, message, strlen (message));

    // now send the BMP
    unsigned char bmp[] = {
        0x42,0x4d,0x5a,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x36,0x00,0x00,0x00,0x28,0x00,
        0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,
        0x00,0x00,0x01,0x00,0x18,0x00,0x00,0x00,
        0x00,0x00,0x24,0x00,0x00,0x00,0x13,0x0b,
        0x00,0x00,0x13,0x0b,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x07,
        0xff,0x07,0x07,0x07,0x07,0x07,0xff,0x00,
        0x00,0x0e,0x07,0x07,0x07,0x66,0x07,0x07,
        0x07,0x07,0x07,0x00,0x00,0x0d,0x07,0x07,
        0x07,0x07,0x07,0x07,0xff,0xff,0xff,0x00,
        0x00,0x0d};

    write (socket, bmp, sizeof(bmp));
}

// start the server listening on the specified port number
int makeServerSocket (int portNumber) {
    // create socket
    int serverSocket = socket (AF_INET, SOCK_STREAM, 0);

    assert (serverSocket >= 0); // error opening socket

    // bind socket to listening port
    struct sockaddr_in serverAddress;
    bzero ((char *) &serverAddress, sizeof (serverAddress));

    serverAddress.sin_family      = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port        = htons (portNumber);

    // let the server start immediately after a previous shutdown
    int optionValue = 1;
    setsockopt (
        serverSocket,
        SOL_SOCKET,
        SO_REUSEADDR,
        &optionValue,
        sizeof(int)
    );

    int bindSuccess =
        bind (
            serverSocket,
            (struct sockaddr *) &serverAddress,
            sizeof (serverAddress)
        );

    assert (bindSuccess >= 0);
    // if this assert fails wait a short while to let the operating
    // system clear the port before trying again

    return serverSocket;
}

// wait for a browser to request a connection,
// returns the socket on which the conversation will take place
int waitForConnection (int serverSocket) {
    // listen for a connection
    const int serverMaxBacklog = 10;
    listen (serverSocket, serverMaxBacklog);

    // accept the connection
    struct sockaddr_in clientAddress;
    socklen_t clientLen = sizeof (clientAddress);
    int connectionSocket =
        accept (
            serverSocket,
            (struct sockaddr *) &clientAddress,
            &clientLen
        );

    assert (connectionSocket >= 0); // error on accept

    return (connectionSocket);
}
