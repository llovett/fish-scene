#include "Fish.h"
#include <GL/glut.h>

Fish::Fish( double size ) {
    this->size = size;
    this->ani_body = this->ani_dorsal = this->ani_fins = this->ani_tail= 0.0;
}

void Fish::render() const {
    renderBody();

    glPushMatrix();
    glTranslatef(0.0, 0.3, 3.0);
    renderDorsal();
    glPopMatrix();

    renderFins();
    renderTail();
}

void Fish::update() {
    // Increment animation counters here
}

void Fish::renderBody() const {
  for ( i=0; i<2; i++ ) {
      glMap2f(GL_MAP2_VERTEX_3,
	      0.0, 1.0, 3, 4,
	      0.0, 1.0, 12, 4,
	      &fishBody[i*4][0][0]);
      glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
      glEvalMesh2(GL_FILL, 0, 10, 0, 10);
  }
}

void Fish::renderDorsal() const {
  for ( i=0; i<2; i++ ) {
      glMap2f(GL_MAP2_VERTEX_3,
	      0.0, 1.0, 3, 4,
	      0.0, 1.0, 12, 2,
	      &fishDorsal[i*2][0][0]);
      glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
      glEvalMesh2(GL_FILL, 0, 10, 0, 10);
  }
}

void Fish::renderFins() const {
    // TODO
}

void Fish::renderTail() const {
    // TODO
}
