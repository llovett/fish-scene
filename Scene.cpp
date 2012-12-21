#include "Scene.h"
#include "Fish.h"
#include <GL/glut.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "TextureLoader.h"
#include <vector>
#include <ctime>

#define FISHROWS 2
#define FISHCOLS 2
#define FISHSTACKS 2
#define SPACING_X 1.5
#define SPACING_Y 4.5
#define SPACING_Z 3

#define POSX 0.0
#define POSY 20.0
#define LAX 0.0
#define LAY 3.0
#define LAZ 3.0

#define RANDOM(start, range) rand()/(float)RAND_MAX * range + start

using namespace std;

vector<Fish*> school;
vector< vector<float> > offsets;


GLfloat Position = 0.0;
GLfloat CameraRot = 0.0;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glTranslatef(0, Position, 0);
    glRotatef(CameraRot, 0, 0, 1);

    for ( int i=0; i<FISHROWS; i++ ) {
    	for ( int j=0; j<FISHCOLS; j++ ) {
	    for ( int k=0; k<FISHSTACKS; k++ ) {
		glPushMatrix();
		int index = i*FISHROWS + j*FISHCOLS + k;
		glTranslatef(i*SPACING_X + offsets[index][0],
			     j*SPACING_Y + offsets[index][1],
			     k*SPACING_Z + offsets[index][2]);
		Fish *f = school[index];
		f->update();
		f->render();
		glPopMatrix();
	    }
    	}
    }
    glPopMatrix();


    if ( CameraRot < 180 ) {
	if ( Position >= 12 ) {
	    CameraRot += 4;
	} else {
	    Position += 0.1;
	}
    }

    glutSwapBuffers();
}

void myReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 45.0, w*1.0/h, 0.5, 200);
    glMatrixMode(GL_MODELVIEW);
}

void myInit() {
    GLfloat ambient[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat position[] = {30.0, 5.0, 10.0, 1.0};
    GLfloat mat_diffuse[] = {0.1, 0.1, 0.9, 1.0};
    GLfloat mat_specular[] = {0.8, 0.8, 0.8, 1.0};
    GLfloat mat_shininess[] = {50.0};

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(POSX, POSY, 5.0, LAX, LAY, LAZ, 0.0,  0.0, 1.0);

    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_MAP2_VERTEX_3);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_MAP2_TEXTURE_COORD_2);

    glClearColor(0.1, 0.6, 0.7, 1.0);
  
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    // Create school of fish, and give them random offsets from each other
    srand( time(NULL) );
    for ( int i=0; i<FISHROWS; i++ ) {
	for ( int j=0; j<FISHCOLS; j++ ) {
	    for ( int k=0; k<FISHSTACKS; k++ ) {
		float size = RANDOM( 0.8, 0.2 );
		school.push_back( new Fish(size) );

		vector<float> fishOffsets;
		for ( int q=0; q<3; q++ ) {
		    fishOffsets.push_back( RANDOM(-0.5, 1) );
		}
		offsets.push_back( fishOffsets );
	    }
	}
    }
}

void myIdle(){
    glutPostRedisplay();
}

// void keyPressed(unsigned char key, int mX, int mY) {
//     switch (key) {
//     case 'w':
// 	PosX -= 0.1;
// 	break;
//     case 's':
// 	PosX += 0.1;
// 	break;
//     case 'a':
// 	PosY -= 0.1;
// 	break;
//     case 'd':
// 	PosY += 0.1;
// 	break;
//     case ' ':
// 	break;
//     }
// }

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow( "Fish" );
    glutReshapeFunc(myReshape); 
    glutIdleFunc(myIdle);
    // glutKeyboardFunc( keyPressed );
    myInit();
    glutDisplayFunc(display);
    glutMainLoop();
}


