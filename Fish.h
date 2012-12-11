/**
 * Fish.h
 *
 * Header file for the Fish model
 *
 * */

#ifndef FISH_H
#define FISH_H

#include <GL/glut.h>

class Fish {
private:
    double size;
    double ani_body, ani_dorsal, ani_fins, ani_tail;
    void renderBody() const;
    void renderDorsal() const;
    void renderFins() const;
    void renderTail() const;
    GLfloat fishDorsal[4][4][3];
    GLfloat fishBody[8][4][3];

    // FISH COMPONENTS
    // --> dorsal fin
    static const GLfloat _fishDorsal[4][4][3] = {
	{ {0.05, 0.0, -1.0}, {0.0, 0.25, 0.1}, {0.0, 3.0, 0.1}, {0.05, 3.0, 0.0} },
	{ {0.05, 0.0, -1.0}, {0.0, 0.25, 1.0}, {0.0, 3.0, 1.0}, {0.05, 3.0, 0.0} },
    
	{ {-0.05, 3.0, 0.0}, {0.0, 3.0, 0.1}, {0.0, 0.25, 0.1}, {-0.05, 0.0, -1.0} },
	{ {-0.05, 3.0, 0.0}, {0.0, 3.0, 1.0}, {0.0, 0.25, 1.0}, {-0.05, 0.0, -1.0} }
    };

    // --> body
    static const GLfloat _fishBody[8][4][3] = {
	{ {0.0, 0.0, 2.0}, {0.0, 1.0, 4.0}, {0.0, 4.0, 3.5}, {0.0, 4.0, 2.0} },
	{ {0.0, 0.0, 2.0}, {-1.0, 1.0, 3.0}, {-0.25, 4.0, 3.0}, {0.0, 4.0, 2.0} },
	{ {0.0, 0.0, 2.0}, {-1.0, 1.0, 2.0}, {-0.25, 4.0, 2.0}, {0.0, 4.0, 2.0} },
	{ {0.0, 0.0, 2.0}, {0.0, 1.0, 1.0}, {0.0, 3.5, 0.5}, {0.0, 4.0, 2.0} },

	{ {0.0, 4.0, 2.0},  {0.0, 4.0, 3.5},    {0.0, 1.0, 4.0}, {0.0, 0.0, 2.0} },
	{ {0.0, 4.0, 2.0},  {0.25, 4.0, 3.0},  {1.0, 1.0, 3.0},{0.0, 0.0, 2.0} },
	{ {0.0, 4.0, 2.0},  {0.25, 4.0, 2.0},  {1.0, 1.0, 2.0},{0.0, 0.0, 2.0} },
	{ {0.0, 4.0, 2.0},  {0.0, 3.5, 0.5},    {0.0, 1.0, 1.0}, {0.0, 0.0, 2.0} },
    };

public:
    Fish( double size );
    void render() const;
    void update();
};

#endif
