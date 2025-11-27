#ifndef Projectile2_H_INCLUDED
#define Projectile2_H_INCLUDED
#include "element.h"
#include "../shapes/Shape.h"
#include "../algif5/algif.h"




//Projection Direction
#define Projectile2_Right 201
#define Projectile2_Left 202




/*
   [Projectile object]
*/
typedef struct _Projectile2
{
    int x, y;          // the position of image
    int width, height; // the width and height of image
    int v;             // the velocity of projectile
    int damage;        // hit strength
    ALLEGRO_BITMAP *img;
    Shape *hitbox; // the hitbox of object




    bool is_enemy_projectile;
} Projectile2;




Elements *New_Projectile2(int label, int x, int y, int v);
void Projectile2_update(Elements *self);
void Projectile2_interact(Elements *self);
void Projectile2_draw(Elements *self);
void Projectile2_destroy(Elements *self);




void _Projectile2_update_position(Elements *self, int dx, int dy);
void _Projectile2_interact_Floor(Elements *self, Elements *tar);
void _Projectile2_interact_Character(Elements *self, Elements *tar);








#endif







