#include "projectile2.h"
#include "charater.h"
#include "character2.h"
#include "enemy.h"
#include "../shapes/Circle.h"
#include "../scene/sceneManager.h"
#include "../scene/gamescene.h"
#include "element.h"

// Constructor
Projectile2::Projectile2(int label, int x, int y, int v)
    : Elements(label), x(x), y(y), v(v), damage(1), is_enemy_projectile(true)
{
    img = al_load_bitmap("assets/image/projectile2.png");

    if (!img) {
        width = height = 16;
    } else {
        width = al_get_bitmap_width(img);
        height = al_get_bitmap_height(img);
    }

    hitbox = new Circle(
        x + width / 2,
        y + height / 2,
        std::min(width, height) / 2
    );
}

Projectile2::~Projectile2() {
    if (img) al_destroy_bitmap(img);
    delete hitbox;
}

void Projectile2::Update() {
    x += v;
    hitbox->update_position(v, 0);

    if (x < -width || x > WIDTH + width)
        dele = true;
}

void Projectile2::Interact()
{
    if (dele || !Elements::scene) return;

    for (Elements* ele : Elements::scene->getAllElements()) {

        if (ele->dele) continue;

        Character*  c1 = dynamic_cast<Character*>(ele);
        Character2* c2 = dynamic_cast<Character2*>(ele);

        if (!c1 && !c2) continue;

        Shape* target_hitbox = c1 ? c1->hitbox : c2->hitbox;

        if (!target_hitbox) continue;

        if (hitbox->overlap(*target_hitbox)) {
            interact_Character(ele);
            return;
        }
    }
}

void Projectile2::interact_Character(Elements* tar)
{
    if (Character* c1 = dynamic_cast<Character*>(tar)) {
        if (c1->blood > 0) c1->blood -= damage;
    }
    else if (Character2* c2 = dynamic_cast<Character2*>(tar)) {
        if (c2->blood > 0) c2->blood -= damage;
    }

    dele = true;
}


void Projectile2::Draw()
{
    if (!img || dele) return;

    if (v < 0)
        al_draw_bitmap(img, x, y, ALLEGRO_FLIP_HORIZONTAL);
    else
        al_draw_bitmap(img, x, y, 0);
}
