#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "mandelbrot.h"

// escapeSteps
#define MIN_ITERATIONS 1
#define MAX_ITERATIONS 256
#define ESCAPE_DISTANCE 2

typedef struct _complex {
    double real;
    double imaginary;
} complex;

static int isBounded(complex num);
static double distanceFromOrigin(complex num);
static complex nextTerm(complex num, complex initial);
static complex complexSquare(complex num);
static double square(double x);

// Server
#define SIMPLE_SERVER_VERSION 1.0
#define REQUEST_BUFFER_SIZE 1000
#define DEFAULT_PORT 1917
#define NUMBER_OF_PAGES_TO_SERVE 10 // server halts after serving this

static void serveHTML(int socket);
static void serveBMP(int socket);
static int makeServerSocket(int portno);
static int waitForConnection(int serverSocket);

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

void writeHeader(FILE *file);

int main(int argc, char *argv[]) {
    printf("************************************\n");
    printf("Starting simple server %f\n", SIMPLE_SERVER_VERSION);
    printf("Serving bmps since 2012\n");

    int serverSocket = makeServerSocket(DEFAULT_PORT);
    printf("Access this server at http://localhost:%d/\n", DEFAULT_PORT);
    printf("************************************\n");

    char request[REQUEST_BUFFER_SIZE];

    int numberServed = 0;
    while (numberServed < NUMBER_OF_PAGES_TO_SERVE) {
        printf ("*** So far served %d pages ***\n", numberServed);

        int connectionSocket = waitForConnection(serverSocket);
        // wait for a request to be sent from a web browser, open a new
        // connection for this conversation

        // read the first line of the request sent by the browser
        int bytesRead;
        bytesRead = read(connectionSocket, request, (sizeof request) - 1);
        assert(bytesRead >= 0);
        // were we able to read any data from the connection?

        // print entire request to the console
        printf(" *** Received http request ***\n %s\n", request);

        //send the browser a simple html page using http
        printf(" *** Sending http response ***\n");
   
        // harrison's magical URL grepper code
        char url[1024];
        sscanf(request, "GET %s HTTP/1.1\n", url);
        printf("\n%s\n", url);
        
        if (strcmp(url, "/" ) == 0) {
            printf("Client is requesting the home page\n");
            serveHTML(connectionSocket);
        } else { 
            printf("Client is requesting a tile\n");
            double x;
            double y;
            int z;

            sscanf(url, "/tile_x-%lf_y%lf_z%d.bmp", &x, &y, &z);

            printf("x: %lf, y:%lf, zoom: %d\n\n", x, y, z); 
    
            serveBMP(connectionSocket); 
        }

        // close the connection after sending the page- keep aust beautiful
        close(connectionSocket);

        numberServed++;
    }

    // close the server connection after we are done- keep aust beautiful
    printf("** shutting down the server **\n");
    close(serverSocket);

    return EXIT_SUCCESS;
}

int escapeSteps(double x, double y) {
    complex initialValue = {x, y};
    complex workingCopy = initialValue;
    int iterations = MIN_ITERATIONS; // Mandelbrot assumes this

    while (isBounded(workingCopy) && iterations < MAX_ITERATIONS) {
        workingCopy = nextTerm(workingCopy, initialValue);
        iterations++;
    }
    return iterations;
}

static int isBounded(complex num) {
    int bounded = 1;
    if (distanceFromOrigin(num) >= ESCAPE_DISTANCE * ESCAPE_DISTANCE) {
        bounded = 0;
    }
    return bounded;
}

static double distanceFromOrigin(complex num) {
    // Distance formula
    return square(num.real) + square(num.imaginary);
}

static complex nextTerm(complex num, complex initial) {
    num = complexSquare(num);
    num.real += initial.real;
    num.imaginary += initial.imaginary;
    return num;
}

static complex complexSquare(complex num) {
    // The square of a complex number resolves to:
    // (x + yi)**2 = (x**2 - y**2) + (2xy)i
    complex temp = num;
    num.real = square(temp.real) - square(temp.imaginary);
    num.imaginary = 2 * temp.real * temp.imaginary;
    return num;
}

static double square(double x) {
    return x * x;
}

static void serveHTML(int socket) {
    char* message;

    // first send the http response header
    message =
        "HTTP/1.0 200 Found\n"
        "Content-Type: text/html\n"
        "\n";
    printf("about to send=> %s\n", message);
    write(socket, message, strlen(message));

    message =
        "<!DOCTYPE html>\n"
        "<script src=\"http://almondbread.cse.unsw.edu.au/tiles.js\"></script>"
        "\n";
    write(socket, message, strlen(message));
}

static void serveBMP(int socket) {
    char* message;

    // first send the http response header
    message = "HTTP/1.0 200 OK\n"
              "Content-Type: image/bmp\n"
              "\n";
    printf("about to send=> %s\n", message);
    write(socket, message, strlen(message));

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

    write(socket, bmp, sizeof(bmp));
}

// start the server listening on the specified port number
static int makeServerSocket(int portNumber) {
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
static int waitForConnection(int serverSocket) {
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

void writeHeader(FILE *file) {
    unsigned short magicNumber = MAGIC_NUMBER;
    fwrite(&magicNumber, sizeof magicNumber, 1, file);

    unsigned int fileSize = OFFSET + (SIZE * SIZE * BYTES_PER_PIXEL);
    fwrite(&fileSize, sizeof fileSize, 1, file);

    unsigned int reserved = 0;
    fwrite(&reserved, sizeof reserved, 1, file);

    unsigned int offset = OFFSET;
    fwrite(&offset, sizeof offset, 1, file);

    unsigned int dibHeaderSize = DIB_HEADER_SIZE;
    fwrite(&dibHeaderSize, sizeof dibHeaderSize, 1, file);

    unsigned int width = SIZE;
    fwrite(&width, sizeof width, 1, file);

    unsigned int height = SIZE;
    fwrite(&height, sizeof height, 1, file);

    unsigned short planes = NUMBER_PLANES;
    fwrite(&planes, sizeof planes, 1, file);

    unsigned short bitsPerPixel = BITS_PER_PIXEL;
    fwrite(&bitsPerPixel, sizeof bitsPerPixel, 1, file);

    unsigned int compression = NO_COMPRESSION;
    fwrite(&compression, sizeof compression, 1, file);

    unsigned int imageSize = (SIZE * SIZE * BYTES_PER_PIXEL);
    fwrite(&imageSize, sizeof imageSize, 1, file);

    unsigned int hResolution = PIX_PER_METRE;
    fwrite(&hResolution, sizeof hResolution, 1, file);

    unsigned int vResolution = PIX_PER_METRE;
    fwrite(&vResolution, sizeof vResolution, 1, file);

    unsigned int numColors = NUM_COLORS;
    fwrite(&numColors, sizeof numColors, 1, file);

    unsigned int importantColors = NUM_COLORS;
    fwrite(&importantColors, sizeof importantColors, 1, file);
}
