#include "Scene.h"
#include "Fish.h"
#include <GL/glut.h>

Fish fish;

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // glPushMatrix();
  // glRotatef(100.0f, 1.0, 0.0, 0.0);
  // glTranslatef(0.0, 0.0, -1.5);
  fish.render();
  // glPopMatrix();

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
  glutPostRedisplay();
}

void keyPressed(int key, int mX, int mY) {
    // TODO: a key was pressed
}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutCreateWindow( "Fish" );
  glutReshapeFunc(myReshape); 
  glutIdleFunc(myIdle);
  glutSpecialFunc( keyPressed );
  myInit();
  glutDisplayFunc(display);

  glutMainLoop();
}


