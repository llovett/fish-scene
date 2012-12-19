/**
 * Fish.h
 *
 * Header file for the Fish model
 *
 * */

#ifndef FISH_H
#define FISH_H

#include <GL/glut.h>

#define TEX_SCALES_LOCATION "scales.ppm"
#define TEX_SCALES_DIM 512
#define TEX_FINS_LOCATION "fishfin.ppm"
#define TEX_FINS_DIM 512

// How quickly to update
#define ANI_BODY_AMNT 0.1

/* Fish class declaration */
class Fish {
private:
    double size;
    double ani_body, ani_dorsal, ani_fins, ani_tail;
    void renderBody() const;
    void renderDorsal() const;
    void renderFin() const;
    void renderTail() const;
    GLfloat fishDorsal[4][4][3];
    GLfloat fishBody[8][4][3];
    GLfloat fishFin[4][3];
    GLfloat fishTail[4][4][3];
    void _this_( double );
    GLfloat *scalesTexture, *finTexture;
    
public:
    Fish( double );
    Fish();
    void render() const;
    void update();
};

/* defines properties for a material */
typedef struct {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
} mProps;

/* defines properties for lighting */
typedef struct {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
} lProps;

/* function declarations */
void setMaterial( mProps* );

#endif
