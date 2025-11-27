#ifndef Projectile3_H_INCLUDED
#define Projectile3_H_INCLUDED
#include "element.h"
#include "../shapes/Shape.h"
#include "../algif5/algif.h"




//Projection Direction
#define Projectile3_Right 201
#define Projectile3_Left 202




/*
   [Projectile object]
*/
typedef struct _Projectile3
{
    int x, y;          // the position of image
    int width, height; // the width and height of image
    int v;             // the velocity of projectile
    int damage;        // hit strength
    ALLEGRO_BITMAP *img;
    Shape *hitbox; // the hitbox of object




    bool is_enemy_projectile;
} Projectile3;




Elements *New_Projectile3(int label, int x, int y, int v);
void Projectile3_update(Elements *self);
void Projectile3_interact(Elements *self);
void Projectile3_draw(Elements *self);
void Projectile3_destroy(Elements *self);




void _Projectile3_update_position(Elements *self, int dx, int dy);
void _Projectile3_interact_Floor(Elements *self, Elements *tar);
void _Projectile3_interact_Character(Elements *self, Elements *tar);








#endif







