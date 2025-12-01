#ifndef BALL_H_INCLUDED
#define BALL_H_INCLUDED
#include "element.h"
#include "../shapes/Shape.h"
/*
   [Ball object]
*/

class Ball : public Elements{

public:
    int x, y; // the position of image
    int r;    // the radius
    Shape *hitbox; // the hitbox of object
    ALLEGRO_COLOR color;

    Ball(int label);
    ~Ball();

    void Draw() override;
    void Interact() override;
    void Update() override;
};

#endif
