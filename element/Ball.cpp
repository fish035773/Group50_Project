/*
#include "Ball.h"
#include "../global.h"
#include "../shapes/Circle.h"
#include "charater.h"
#include "tree.h"
#include "../scene/sceneManager.h"
#include "../scene/gamescene.h"
#include <allegro5/allegro_primitives.h>

Ball::Ball(int label): Elements(label){
    this->x = mouse.x;
    this->y = mouse.y;
    this->r = 10;
    this->color = al_map_rgb(255, 0, 0);
    this->hitbox = new Circle(this->x, this->y, this->r);

    this->inter_obj[this->inter_len++] = Character_L;
    this->inter_obj[this->inter_len++] = Tree_L;
}
    
Ball::~Ball(){
    delete hitbox;
}

void Ball::Update() {
    this->hitbox->update_center_x(mouse.x - x);
    this->hitbox->update_center_y(mouse.y - x);

    x = mouse.x;
    y = mouse.y;
}

void Ball::Interact() {
    ElementVec labelEle = _Get_label_elements(scene, Character_L);
    Character* Obj1 = (Character*)labelEle.arr[0];

    labelEle = _Get_label_elements(scene, Tree_L);

    if (hitbox->overlap(*Obj1->hitbox)) {
        color = al_map_rgb(0, 255, 0);
    } else if (hitbox->overlap(*Obj2->hitbox)) {
        color = al_map_rgb(0, 0, 255);
    } else {
        color = al_map_rgb(255, 0, 0);
    }
}

void Ball::Draw() {
    al_draw_circle(x, y, r, color, 10);
}
*/