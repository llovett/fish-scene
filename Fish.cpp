#include "Fish.h"
#include "Matrix.h"
#include <GL/glut.h>

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

GLfloat _fishFin[2][4][3] = {
    { {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.5, 1.2, -1.0}, {0.2, -0.2, -1.0} },
    { {0.2, -0.2, -1.0}, {0.5, 1.2, -1.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0} }
};

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
    for ( int i=0; i<2; i++ ) {
	for ( int j=0; j<4; j++ ) {
	    for ( int k=0; k<3; k++ ) {
		fishFin[i][j][k] = size * _fishFin[i][j][k];
	    }
	}
    }
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

    renderFins();
    renderTail();
}

void Fish::update() {
    // Increment animation counters here
}

void Fish::renderBody() const {
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
  for ( int i=0; i<2; i++ ) {
      glMap2f(GL_MAP2_VERTEX_3,
	      0.0, 1.0, 3, 4,
	      0.0, 1.0, 12, 2,
	      &fishDorsal[i*2][0][0]);
      glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
      glEvalMesh2(GL_FILL, 0, 10, 0, 10);
  }
}

void Fish::renderFins() const {
    // Calculate the normals
    GLfloat v1[3] = {
	fishFin[0][0][0] - fishFin[0][1][0],
	fishFin[0][0][1] - fishFin[0][1][1],
	fishFin[0][0][2] - fishFin[0][1][2]
    };
    GLfloat v2[3] = {
	fishFin[0][1][0] - fishFin[0][2][0],
	fishFin[0][1][1] - fishFin[0][2][1],
	fishFin[0][1][2] - fishFin[0][2][2]
    };
    GLfloat *normal = crossProduct(v1, v2);
    
    for ( int i=0; i<2; i++ ) {
	glBegin( GL_POLYGON );
	for ( int j=0; j<4; j++ ) {
	    glVertex3f( fishFin[i][j][0],
			fishFin[i][j][1],
			fishFin[i][j][3] );
	}
	glEnd();
    }

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
