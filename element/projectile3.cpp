#include "projectile3.h"
#include "charater.h"
#include "Character2.h"
#include "enemy.h"
#include "../shapes/Circle.h"
#include "../scene/gamescene.h" // for element label
#include "../scene/sceneManager.h" // for scene variable

Projectile3::Projectile3(int label, int x, int y, int v)
    : Elements(label), x(x), y(y), v(v)
{
    img = al_load_bitmap("assets/image/projectile3.png");

    if (!img) {
        width = height = 16;
    } else {
        width = al_get_bitmap_width(img);
        height = al_get_bitmap_height(img);
    }

    damage = 5;
    is_enemy_projectile = true;

    hitbox = new Circle(
        x + width / 2,
        y + height / 2,
        std::min(width, height) / 2
    );
}

Projectile3::~Projectile3() {
    if (img) al_destroy_bitmap(img);
    if (hitbox) delete hitbox;
}

void Projectile3::Update() {
    update_position(v, 0);
}

void Projectile3::update_position(int dx, int dy) {
    x += dx;
    y += dy;
    hitbox->update_position(dx, 0);
}

void Projectile3::Interact()
{
    if (dele || !scene) return;

    // === check collision with Characters ===
    for (Elements* ele : scene->getAllElements()) {

        if (ele->dele) continue;

        Character* c1 = dynamic_cast<Character*>(ele);
        Character2* c2 = dynamic_cast<Character2*>(ele);

        if (!c1 && !c2)
            continue; // not a character

        Shape* target_hitbox = nullptr;
        if (c1) target_hitbox = c1->hitbox;
        if (c2) target_hitbox = c2->hitbox;

        if (!target_hitbox) continue;

        // --- collision check ---
        if (hitbox->overlap(*target_hitbox)) {

            if (c1) interact_Character(c1);
            if (c2) interact_Character(c2);

            dele = true;
            return;   // projectile disappears after hit
        }
    }

    // === delete if out of bounds ===
    if (x < -width || x > WIDTH + width)
        dele = true;
}


void Projectile3::interact_Floor(Elements *tar) {
    if (x < -width)
        dele = true;
    else if (x > WIDTH + width)
        dele = true;
}

void Projectile3::interact_Character(Elements *tar)
{
    if (tar->label == Character_L) {
        Character* p = dynamic_cast<Character*>(tar);
        if (p->hitbox->overlap(*hitbox) && p->blood > 0) {
            dele = true;
            p->blood -= damage;
        }
    }
    else if (tar->label == Character2_L) {
        Character* p2 = dynamic_cast<Character*>(tar);
        if (p2->hitbox->overlap(*hitbox) && p2->blood > 0) {
            dele = true;
            p2->blood -= damage;
        }
    }
}

void Projectile3::Draw() {
    if (v < 0)
        al_draw_bitmap(img, x, y, ALLEGRO_FLIP_HORIZONTAL);
    else
        al_draw_bitmap(img, x, y, 0);
}

