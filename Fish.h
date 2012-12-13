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
    void _this_( double );

public:
    Fish( double );
    Fish();
    void render() const;
    void update();
};

#endif
