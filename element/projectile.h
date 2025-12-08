#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "../shapes/Shape.h"
#include "../element/element.h"
#include "Character2.h"

class Projectile : public Elements {
public:
    // projectile type IDs (same as your label enum)
    enum ProjectileType { 
        Projectile_X, Projectile_C, Projectile_V,
        Projectile_K, Projectile_L, Projectile_J
    };

    // constructor / destructor
    Projectile(int label, int x, int y, int v, void* owner);
    ~Projectile();

    // override from Elements
    void Update() override;
    void Interact() override;
    void Draw() override;

    // position
    int x, y;

    // size
    int width, height;

    // image
    ALLEGRO_BITMAP* img = nullptr;

    // logic
    int v;          // velocity
    int damage;     // damage based on projectile type
    int origin;     // 1 = Character, 2 = Character2
    void* owner;    // pointer to Character or Character2

    // collision shape
    Shape* hitbox = nullptr;

    // helper
    void interactEnemy(Elements* e);
    void interactEnemy2(Elements* e);
    void interactEnemy3(Elements* e);
};
