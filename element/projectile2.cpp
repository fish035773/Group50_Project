#include "projectile2.h"
#include "charater.h"
#include "Character2.h"
#include "enemy.h"
#include "../shapes/Circle.h"
#include "../scene/gamescene.h" // for element label
#include "../scene/sceneManager.h" // for scene variable

Projectile2::Projectile2(int label, int x, int y, int v): Elements(label), x(x), y(y), v(v){
    img = al_load_bitmap("assets/image/projectile2.png");
    width = al_get_bitmap_width(img);
    height = al_get_bitmap_height(img);

    damage = 1;
    is_enemy_projectile = true;

    hitbox = new Circle(
        x + width / 2,
        y + height / 2,
        std::min(width, height) / 2
    );

    inter_obj[inter_len++] = Floor_L;
    inter_obj[inter_len++] = Character_L;
    inter_obj[inter_len++] = Character2_L;
}

Projectile2::~Projectile2() {
    if (img) al_destroy_bitmap(img);
    if (hitbox) delete hitbox;
}

void Projectile2::Update() {
    update_position(v, 0);
}

void Projectile2::update_position(int dx, int dy) {
    x += dx;
    y += dy;

    hitbox->update_center_x(dx);
    hitbox->update_center_y(dy);
}

void Projectile2::Interact()
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

void Projectile2::interact_Floor(Elements *tar) {
    if (x < -width)
        dele = true;
    else if (x > WIDTH + width)
        dele = true;
}

void Projectile2::interact_Character(Elements *tar)
{
    if (tar->label == Character_L) {
        Character* p = (Character*)tar->pDerivedObj;
        if (p->hitbox->overlap(*hitbox) && p->blood > 0) {
            dele = true;
            p->blood -= damage;
        }
    }
    else if (tar->label == Character2_L) {
        Character2* p2 = (Character2*)tar->pDerivedObj;
        if (p2->hitbox->overlap(*hitbox) && p2->blood > 0) {
            dele = true;
            p2->blood -= damage;
        }
    }
}

void Projectile2::Draw() {
    if (v < 0)
        al_draw_bitmap(img, x, y, ALLEGRO_FLIP_HORIZONTAL);
    else
        al_draw_bitmap(img, x, y, 0);
}
