#include "TextureLoader.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

/**
 * Returns a 2D array of colors that are in the PPM picture.
 * */
GLfloat *readPPM( const char *filename ) {

    FILE *ppm = fopen( filename, "r" );
    if ( !ppm ) {
	perror("fopen");
	exit( 1 );
    }
    char *line = (char *)malloc(256 * sizeof(char) );
    long unsigned int linesize = 256;

    // Move past header
    getline( &line, &linesize, ppm );

    // Move past comments
    int eoc = 0;
    while ( !eoc ) {
	getline( &line, &linesize, ppm );
	if ( line[0] != '#' ) eoc = 1;
    }

    // Read the dimensions of the image
    int width, height;
    sscanf( line, "%d %d", &width, &height );

    // Malloc-ing the pixel array
    GLfloat *matrix = (GLfloat *)malloc( width * height * 3 * sizeof(GLfloat) );
    if ( !matrix ) {
	perror("malloc");
	cerr << "Could not create image texture!" << endl;
	exit( 1 );
    }

    // Move past max luminosity (it's 256)
    getline( &line, &linesize, ppm );

    // Read colors
    for ( int i=0; i<height; i++ ) {
	for ( int j=0; j<width; j++ ) {
	    int red, green, blue;
	    getline( &line, &linesize, ppm );
	    red = atoi( line );
	    getline( &line, &linesize, ppm );
	    green = atoi( line );
	    getline( &line, &linesize, ppm );
	    blue = atoi( line );
	    matrix[3*i*height + j*3] = red / (GLfloat)255.0;
	    matrix[3*i*height + j*3 + 1] = green / (GLfloat)255.0;
	    matrix[3*i*height + j*3 + 2] = blue / (GLfloat)255.0;
	}
    }

    free( line );
    
    fclose( ppm );

    return matrix;
}
