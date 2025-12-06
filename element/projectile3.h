#pragma once
#include "element.h"
#include "../shapes/Shape.h"
#include "../algif5/algif.h"

class Projectile3 : public Elements {
public:
    int x, y;
    int width, height;
    int v;
    int damage;     

    ALLEGRO_BITMAP* img;
    Shape* hitbox;

    bool is_enemy_projectile;

    Projectile3(int label, int x, int y, int v);
    ~Projectile3();

    void Update() override;
    void Interact() override;
    void Draw() override;

    void update_position(int dx, int dy);
    void interact_Floor(Elements* tar);
    void interact_Character(Elements* tar);
    void interact_Character2(Elements* tar);
};






