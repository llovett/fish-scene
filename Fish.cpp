#include "Fish.h"
#include "Matrix.h"
#include <GL/glut.h>
#include "TextureLoader.h"
#include <cmath>

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

mProps outerEyeMaterial = {
    { 0.8, 0.0, 0.0, 1.0 },
    { 0.8, 0.8, 0.8, 1.0 },
    { 0.7, 0.7, 0.7, 1.0 },
    50.0
};

mProps innerEyeMaterial = {
    { 0.0, 0.0, 0.0, 1.0 },
    { 0.0, 0.0, 0.0, 1.0 },
    { 0.7, 0.7, 0.7, 1.0 },
    50.0
};    

// These are used to model parts of the fish's body
GLfloat _fishDorsal[2][4][3] = {
    { {0.0, 0.0, -1.0}, {0.0, 1, 0.5}, {0.0, 3.0, 0.3}, {0.0, 3.5, -0.45} },//{0.0, 3.6, -0.57} },
    { {0.0, 0.0, -0.7}, {0.0, 1, 1.3}, {0.0, 3.0, 1.0}, {0.0, 3.5, -0.45} }//{0.0, 3.6, -0.57} },
};

GLfloat _fishBody[8][4][3] = {
    { {0.0, 0.0, 2.0}, {0.0, 1.0, 4.0}, {0.0, 3.0, 4.0}, {0.0, 4.0, 2.0} },
    { {0.0, 0.0, 2.0}, {-1.0, 1.0, 3.0}, {-0.25, 3.0, 3.0}, {0.0, 4.0, 2.0} },
    { {0.0, 0.0, 2.0}, {-1.0, 1.0, 2.0}, {-0.25, 3.0, 2.0}, {0.0, 4.0, 2.0} },
    { {0.0, 0.0, 2.0}, {0.0, 1.0, 1.0}, {0.0, 3.0, 0.5}, {0.0, 4.0, 2.0} },

    { {0.0, 4.0, 2.0},  {0.0, 3.0, 4.0},    {0.0, 1.0, 4.0}, {0.0, 0.0, 2.0} },
    { {0.0, 4.0, 2.0},  {0.25, 3.0, 3.0},  {1.0, 1.0, 3.0},{0.0, 0.0, 2.0} },
    { {0.0, 4.0, 2.0},  {0.25, 3.0, 2.0},  {1.0, 1.0, 2.0},{0.0, 0.0, 2.0} },
    { {0.0, 4.0, 2.0},  {0.0, 3.0, 0.5},    {0.0, 1.0, 1.0}, {0.0, 0.0, 2.0} },
};

// N.B., this needs to be transposed
GLfloat _fishTail[4][4][3] = {
    { {0.05, 0.0, -1.0}, {0.0, 0.25, 0.5}, {0.0, 3.0, 0.3}, {0.05, 3.6, -0.57} },
    { {0.05, 0.0, -0.7}, {0.0, 0.25, 1.3}, {0.0, 3.0, 1.0}, {0.05, 3.6, -0.57} },
    
    { {-0.05, 3.6, -0.57}, {0.0, 3.0, 0.3}, {0.0, 0.25, 0.5}, {-0.05, 0.0, -1.0} },
    { {-0.05, 3.6, -0.57}, {0.0, 3.0, 1.0}, {0.0, 0.25, 1.3}, {-0.05, 0.0, -0.7} }
};

GLfloat _fishFin[4][3] = { {0.0, 0.0, 0.0}, {0.0, 0.8, 0.0}, {0.35, 0.9, -0.8}, {0.2, -0.2, -0.8} };

// For modeling quadric objects
GLUquadricObj *quad;

// For tracking animation
GLfloat EyeRot;
GLfloat FinRot;
GLfloat FinPos;
GLfloat TailRot;
GLfloat TailPos;
GLfloat EyePos;
GLfloat FinFlap;

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
    this->ani_body = this->ani_fins = 0.0;

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
    // --> tail
    for ( int i=0; i<4; i++ ) {
	for ( int j=0; j<4; j++ ) {
	    for ( int k=0; k<3; k++ ) {
		fishTail[i][j][k] = size * _fishTail[i][j][k];
	    }
	}
    }

    // Load textures
    scalesTexture = readPPM( TEX_SCALES_LOCATION );
    finTexture = readPPM( TEX_FINS_LOCATION );

    // Initialize quadrics
    quad = gluNewQuadric();
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
    glTranslatef(0.0, 0.0, this->size * 3.0);
    renderDorsal();
    glPopMatrix();

    glPushMatrix();
    glTranslatef( -0.2 - FinPos, 2.2, 1.6 );
    glRotatef(FinRot + 10, 0, 0, 1);
    glRotatef(FinFlap, 0, 1, 0);
    renderFin();
    glPopMatrix();

    glPushMatrix();
    glTranslatef( -0.35 - FinPos, 2.8, 0.9 );
    glRotatef(FinRot, 0, 0, 1);
    glRotatef(-FinFlap, 0, 1, 0);
    glTranslatef(-0.2, 0.2, 0.8);
    glRotatef( 180.0, 0.0, 0.0, 1.0 );
    renderFin();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(TailPos, 0, 0);
    glRotatef(-TailRot, 0, 0, 1);
    renderTail();
    glPopMatrix();
}

void Fish::update() {
    // Increment animation counters here
    this->ani_body += ANI_BODY_AMNT;
    this->ani_fins += ANI_FINS_AMNT;

    // Redefine splines
    // --> fish body
    for ( int i=0; i<4; i++ ) {
	GLfloat origVal = _fishBody[i][1][0] * this->size;
	this->fishBody[i][1][0] = origVal + sin( this->ani_body ) - 0.5;
	origVal = _fishBody[i][2][0] * this->size;
	this->fishBody[i][2][0] = origVal - sin( this->ani_body ) - 0.5;
    }
    for ( int i=4; i<8; i++ ) {
	GLfloat origVal = _fishBody[i][1][0] * this->size;
	this->fishBody[i][1][0] = origVal - sin( this->ani_body ) - 0.5;
	origVal = _fishBody[i][2][0] * this->size;
	this->fishBody[i][2][0] = origVal + sin( this->ani_body ) - 0.5;
    }
    // --> dorsal fin
    for ( int i=0; i<2; i++ ) {
	GLfloat origVal = _fishDorsal[i][3][0] * this->size;
	this->fishDorsal[i][3][0] = origVal - 0.2*sin( this->ani_body ) - 0.2;
	origVal = _fishDorsal[i][1][0] * this->size;
	this->fishDorsal[i][1][0] = origVal + sin( this->ani_body ) - 0.5;
	origVal = _fishDorsal[i][2][0] * this->size;
	this->fishDorsal[i][2][0] = origVal - 0.75*sin( this->ani_body ) - 0.35;
    }

    // Update rotations for fins, eyes, tail, etc.
    // --> eyes
    EyeRot = (sin( this->ani_body ) - 0.5) * 10.0;
    EyePos = (sin( this->ani_body ) - 0.5) * 0.2;
    FinRot = (sin( this->ani_body ) - 0.5) * 15.0;
    FinPos = (sin( this->ani_body ) - 0.5) * 0.15;
    TailRot = (sin( this->ani_body ) - 0.5) * 15.0;
    TailPos = (sin( this->ani_body ) - 0.5) * 0.1;
    FinFlap = (sin( this->ani_fins ) - 0.5) * 5;
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

    //////////// RIGHT EYE
    glPushMatrix();
    glTranslatef(-EyePos - 0.15, 2.8, 2.5);
    glRotatef(80.0, 0, 1, 0);
    glRotatef(-8.0 - EyeRot, 1, 0, 0);

    // outer eye
    setMaterial( &outerEyeMaterial );
    gluDisk( quad, 0, size * 0.4, 40, 40 );
    
    // inner eye
    setMaterial( &innerEyeMaterial );
    glPushMatrix();
    glTranslatef(0, 0, 0.01);
    gluDisk( quad, 0, size * 0.2, 40, 40 );
    glPopMatrix();
    glPopMatrix();

    //////////// LEFT EYE
    glPushMatrix();
    glTranslatef(-EyePos - 0.695, 2.8, 2.5);
    glRotatef(280.0, 0, 1, 0);
    glRotatef(-8.0 + EyeRot, 1, 0, 0);

    // outer eye
    setMaterial( &outerEyeMaterial );
    gluDisk( quad, 0, size * 0.4, 40, 40 );
    
    // inner eye
    setMaterial( &innerEyeMaterial );
    glPushMatrix();
    glTranslatef(0, 0, 0.01);
    gluDisk( quad, 0, size * 0.2, 40, 40 );
    glPopMatrix();
    glPopMatrix();

}

void Fish::renderDorsal() const {
    setupSplineTexture( finTexture, TEX_FINS_DIM, TEX_FINS_DIM, GL_REPLACE );
    glMap2f(GL_MAP2_VERTEX_3,
	    0.0, 1.0, 3, 4,
	    0.0, 1.0, 12, 2,
	    &fishDorsal[0][0][0]);
    glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
    glEvalMesh2(GL_FILL, 0, 10, 0, 10);
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
	glTexCoord2f( sAndt[j][0], sAndt[j][1] );
	glVertex3f( fishFin[j][0],
		    fishFin[j][1],
		    fishFin[j][2] );
    }
    glEnd();

    delete normal;
}

void Fish::renderTail() const {
    glPushMatrix();
    glTranslatef(0, -0.2, 3.9);
    glRotatef(270, 1, 0, 0);

    setupSplineTexture( finTexture, TEX_FINS_DIM, TEX_FINS_DIM, GL_REPLACE );
    for ( int i=0; i<2; i++ ) {
	glMap2f(GL_MAP2_VERTEX_3,
		0.0, 1.0, 3, 4,
		0.0, 1.0, 12, 2,
		&fishTail[i*2][0][0]);
	glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
	glEvalMesh2(GL_FILL, 0, 10, 0, 10);
    }

    glPopMatrix();
}

void setMaterial( mProps *material ) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, material->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material->specular);
    glMaterialf(GL_FRONT, GL_SHININESS, material->shininess);
}
