/*       testpatch.c 
 *
 *  This displays two bicubic patches with Gouraud shading
 */

#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#define PI 3.14159

/* This is enough data for two Bezier patches.
   Note the order of the control points: left to right, bottom to top
 * ordering leads to a normal vector pointing out at the viewer. 
 */
GLfloat fishBody[8][4][3] = {
    /* x = out/in,
     * y = left/right,
     * z = up/down
     * */
    { {0.0, 0.0, 2.0}, {0.0, 1.0, 4.0}, {0.0, 4.0, 3.5}, {0.0, 4.0, 2.0} },
    { {0.0, 0.0, 2.0}, {-1.0, 1.0, 3.0}, {-0.25, 4.0, 3.0}, {0.0, 4.0, 2.0} },
    { {0.0, 0.0, 2.0}, {-1.0, 1.0, 2.0}, {-0.25, 4.0, 2.0}, {0.0, 4.0, 2.0} },
    { {0.0, 0.0, 2.0}, {0.0, 1.0, 1.0}, {0.0, 3.5, 0.5}, {0.0, 4.0, 2.0} },

    { {0.0, 4.0, 2.0},  {0.0, 4.0, 3.5},    {0.0, 1.0, 4.0}, {0.0, 0.0, 2.0} },
    { {0.0, 4.0, 2.0},  {0.25, 4.0, 3.0},  {1.0, 1.0, 3.0},{0.0, 0.0, 2.0} },
    { {0.0, 4.0, 2.0},  {0.25, 4.0, 2.0},  {1.0, 1.0, 2.0},{0.0, 0.0, 2.0} },
    { {0.0, 4.0, 2.0},  {0.0, 3.5, 0.5},    {0.0, 1.0, 1.0}, {0.0, 0.0, 2.0} },

    /**		 
     *	  .   @	.
     *	       	 
     *   @     	 @
     *	  .  @ 	. 
     *
     * */
};

GLfloat fishDorsal[4][4][3] = {
    { {0.05, 0.0, -1.0}, {0.0, 0.25, 0.1}, {0.0, 3.0, 0.1}, {0.05, 3.0, 0.0} },
    { {0.05, 0.0, -1.0}, {0.0, 0.25, 1.0}, {0.0, 3.0, 1.0}, {0.05, 3.0, 0.0} },

    { {-0.05, 3.0, 0.0}, {0.0, 3.0, 0.1}, {0.0, 0.25, 0.1}, {-0.05, 0.0, -1.0} },
    { {-0.05, 3.0, 0.0}, {0.0, 3.0, 1.0}, {0.0, 0.25, 1.0}, {-0.05, 0.0, -1.0} }
};

GLfloat theta = 0.0;

void display(void) {
  int i;
  GLfloat patch[4][4][3];
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  glTranslatef(0.0, 4.0, 0.0);
  glRotatef(theta, 0.0, 0.0, 1.0);
  glTranslatef(0.0, -4.0, 0.0);

  for ( i=0; i<2; i++ ) {
      glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 4,&fishBody[i*4][0][0]);
      glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
      glEvalMesh2(GL_FILL, 0, 10, 0, 10);

      glPushMatrix();
      glTranslatef(0.0, 0.3, 3.0);
      glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 2, &fishDorsal[i*2][0][0]);
      glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
      glEvalMesh2(GL_FILL, 0, 10, 0, 10);
      glPopMatrix();
  }


  glPopMatrix();
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
  gluLookAt(10.0, 1.0, 5.0, 0.0, 3.0, 3.0, 0.0,  0.0, 1.0);

  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_MAP2_VERTEX_3);
  glEnable(GL_NORMALIZE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glClearColor(1.0, 1.0, 1.0, 1.0);
  
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  glLightfv(GL_LIGHT0, GL_POSITION, position);

  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

}

void myIdle(){
  theta += 0.01;
  glutPostRedisplay();
}


int main(int argc, char *argv[]) {
  int i, j, k, m, n;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutCreateWindow( "test" );
  glutReshapeFunc(myReshape); 
  glutIdleFunc(myIdle);
  myInit();
  glutDisplayFunc(display);

  glutMainLoop();
}

