#include "Fish.h"
#include "Matrix.h"
#include <GL/glut.h>
#include "TextureLoader.h"

// Materials used to render the Fish
mProps bodyMaterial = {
    { 0.0, 0.0, 0.3, 1.0 },
    { 0.0, 0.0, 0.9, 1.0 },
    { 0.7, 0.7, 0.9, 1.0 },
    40.0
};

mProps finMaterial = {
    { 0.0, 0.2, 0.4, 1.0 },
    { 0.0, 0.4, 0.8, 1.0 },
    { 0.2, 0.2, 0.2, 1.0 },
    20.0
};

// These are used to model parts of the fish's body
GLfloat _fishDorsal[4][4][3] = {
    { {0.05, 0.0, -1.0}, {0.0, 0.25, 0.1}, {0.0, 3.0, 0.1}, {0.05, 3.0, 0.0} },
    { {0.05, 0.0, -1.0}, {0.0, 0.25, 1.0}, {0.0, 3.0, 1.0}, {0.05, 3.0, 0.0} },
    
    { {-0.05, 3.0, 0.0}, {0.0, 3.0, 0.1}, {0.0, 0.25, 0.1}, {-0.05, 0.0, -1.0} },
    { {-0.05, 3.0, 0.0}, {0.0, 3.0, 1.0}, {0.0, 0.25, 1.0}, {-0.05, 0.0, -1.0} }
};

GLfloat _fishBody[8][4][3] = {
    { {0.0, 0.0, 2.0}, {0.0, 1.0, 4.0}, {0.0, 4.0, 3.5}, {0.0, 4.0, 2.0} },
    { {0.0, 0.0, 2.0}, {-1.0, 1.0, 3.0}, {-0.25, 4.0, 3.0}, {0.0, 4.0, 2.0} },
    { {0.0, 0.0, 2.0}, {-1.0, 1.0, 2.0}, {-0.25, 4.0, 2.0}, {0.0, 4.0, 2.0} },
    { {0.0, 0.0, 2.0}, {0.0, 1.0, 1.0}, {0.0, 3.5, 0.5}, {0.0, 4.0, 2.0} },

    { {0.0, 4.0, 2.0},  {0.0, 4.0, 3.5},    {0.0, 1.0, 4.0}, {0.0, 0.0, 2.0} },
    { {0.0, 4.0, 2.0},  {0.25, 4.0, 3.0},  {1.0, 1.0, 3.0},{0.0, 0.0, 2.0} },
    { {0.0, 4.0, 2.0},  {0.25, 4.0, 2.0},  {1.0, 1.0, 2.0},{0.0, 0.0, 2.0} },
    { {0.0, 4.0, 2.0},  {0.0, 3.5, 0.5},    {0.0, 1.0, 1.0}, {0.0, 0.0, 2.0} },
};

// Used to map textures onto splines
float texel[2][2][2] = {
    { {0.0, 0.0}, {1.0, 0.0} },
    { {0.0, 1.0}, {1.0, 1.0} }
};

GLfloat _fishFin[4][3] = { {0.0, 0.0, 0.0}, {0.0, 0.8, 0.0}, {0.35, 0.9, -0.8}, {0.2, -0.2, -0.8} };

void setupPolygonTexture( GLfloat *pixels, int cols, int rows, GLint modulation ) {
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, modulation );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, cols, rows, 0, GL_RGB, GL_FLOAT, pixels );
}

void setupSplineTexture( GLfloat *pixels, int cols, int rows, GLint modulation ) {
    setupPolygonTexture( pixels, cols, rows, modulation );
    float texel[2][2][2] = {
	{ {0.0, 0.0}, {0.0, 1.0} },
	{ {1.0, 0.0}, {1.0, 1.0} }
    };
    glMap2f( GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2, 2, 0.0, 1.0, 4, 2, &texel[0][0][0] );
}

void Fish::_this_( double size ) {
    this->size = size;
    this->ani_body = this->ani_dorsal = this->ani_fins = this->ani_tail= 0.0;

    // CONSTRUCT PARTS OF THE FISH
    // --> body
    for ( int i=0; i<8; i++ ) {
	for ( int j=0; j<4; j++ ) {
	    for ( int k=0; k<3; k++ ) {
		fishBody[i][j][k] = size * _fishBody[i][j][k];
	    }
	}
    }
    // --> dorsal fin
    for ( int i=0; i<4; i++ ) {
	for ( int j=0; j<4; j++ ) {
	    for ( int k=0; k<3; k++ ) {
		fishDorsal[i][j][k] = size * _fishDorsal[i][j][k];
	    }
	}
    }
    // --> side fins
    for ( int j=0; j<4; j++ ) {
	for ( int k=0; k<3; k++ ) {
	    fishFin[j][k] = size * _fishFin[j][k];
	}
    }

    // Load textures
    scalesTexture = readPPM( TEX_SCALES_LOCATION );
    finTexture = readPPM( TEX_FINS_LOCATION );
}

Fish::Fish( double size ) {
    _this_( size );
}

Fish::Fish( ) {
    _this_( 1.0 );
}

void Fish::render() const {
    setMaterial( &bodyMaterial );
    renderBody();

    setMaterial( &finMaterial );
    glPushMatrix();
    glTranslatef(0.0, this->size * 0.3, this->size * 3.0);
    renderDorsal();
    glPopMatrix();

    glPushMatrix();
    glTranslatef( 0.15, 2.2, 1.6 );
    renderFin();
    glPopMatrix();

    glPushMatrix();
    glTranslatef( -0.15, 3.0, 1.6 );
    glRotatef( 180.0, 0.0, 0.0, 1.0 );
    renderFin();
    glPopMatrix();

    renderTail();
}

void Fish::update() {
    // Increment animation counters here
}

void Fish::renderBody() const {
    setupSplineTexture( scalesTexture, TEX_SCALES_DIM, TEX_SCALES_DIM, GL_MODULATE );
    for ( int i=0; i<2; i++ ) {
	glMap2f(GL_MAP2_VERTEX_3,
		0.0, 1.0, 3, 4,
		0.0, 1.0, 12, 4,
		&fishBody[i*4][0][0]);
	glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
	glEvalMesh2(GL_FILL, 0, 10, 0, 10);
    }
}

void Fish::renderDorsal() const {
    setupSplineTexture( finTexture, TEX_FINS_DIM, TEX_FINS_DIM, GL_REPLACE );
    for ( int i=0; i<2; i++ ) {
	glMap2f(GL_MAP2_VERTEX_3,
		0.0, 1.0, 3, 4,
		0.0, 1.0, 12, 2,
		&fishDorsal[i*2][0][0]);
	glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
	glEvalMesh2(GL_FILL, 0, 10, 0, 10);
    }
}

void Fish::renderFin() const {
    // Calculate the normals
    GLfloat v1[3] = {
	fishFin[0][0] - fishFin[1][0],
	fishFin[0][1] - fishFin[1][1],
	fishFin[0][2] - fishFin[1][2]
    };
    GLfloat v2[3] = {
	fishFin[1][0] - fishFin[2][0],
	fishFin[1][1] - fishFin[2][1],
	fishFin[1][2] - fishFin[2][2]
    };
    GLfloat *normal = crossProduct(v1, v2);

    setupPolygonTexture( finTexture, TEX_FINS_DIM, TEX_FINS_DIM, GL_REPLACE );
    glNormal3f( -normal[0], -normal[1], -normal[2] );
    glBegin( GL_POLYGON );
    float sAndt[4][2] = { {0.0,1.0}, {0.0, 0.0}, {1.0,0.0}, {1.0,1.0} };
    for ( int j=0; j<4; j++ ) {
	glTexCoord2f( sAndt[j][0], sAndt[j][1] );//j/2, j%2 );
	glVertex3f( fishFin[j][0],
		    fishFin[j][1],
		    fishFin[j][2] );
    }
    glEnd();

    delete normal;
}

void Fish::renderTail() const {
    // TODO
}

void setMaterial( mProps *material ) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, material->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material->specular);
    glMaterialf(GL_FRONT, GL_SHININESS, material->shininess);
}
