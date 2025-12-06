#include "projectile2.h"
#include "charater.h"
#include "Character2.h"
#include "enemy.h"

#include "../shapes/Circle.h"
#include "../scene/sceneManager.h"
#include "../scene/gamescene.h"
#include "element.h"
#include <allegro5/allegro_primitives.h>

// ----------------------------------------
// Constructor
// ----------------------------------------
Projectile2::Projectile2(int label, int x, int y, int v)
    : Elements(label), x(x), y(y), v(v),
      damage(1), is_enemy_projectile(true)
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
    printf("[Projectile] Destroyed projectile label=%d\n", label);
}

void Projectile2::Update() {
    if (dele) return;
    x += v;
    
    if (hitbox)
        hitbox->update_position(v, 0);

    if (x < 0 || x > 900) {
        dele = true;
        printf("[Projectile] Out of bounds → dele set to true\n");
    }else {
        Circle* c = static_cast<Circle*>(hitbox);
        //printf("X: %d, %lf Y: %d, %lf\n", x, c->center_x(), y, c->center_y());
    }
}

// ----------------------------------------
// Interact: C++ 版本正確 hitbox 判定
// ----------------------------------------
void Projectile2::Interact(){
    if (dele || !scene) return;

    for (Elements* ele : scene->getAllElements()) {
        if (ele->dele) continue;
        if (ele->label != Character_L && ele->label != Character2_L) continue;

        if (Character* c1 = dynamic_cast<Character*>(ele)) {
            if (hitbox->overlap(*c1->hitbox)){
                dele = true;
                interact_Character(c1);
                return;
            }
        }
        if (Character2* c2 = dynamic_cast<Character2*>(ele)) {
            if (hitbox->overlap(*c2->hitbox)){
                dele = true;
                interact_Character2(c2);
                return;
            }
            continue;
        }
    }
}

void Projectile2::interact_Character(Elements *tar){
    Character* ch = dynamic_cast<Character*>(tar);
    if(!ch) return;

    int dam = ch->blood - damage;
    ch->blood = (dam < 0) ? 0 : dam;
    
    printf("[Projectile] Character1 hit! HP = %d\n", ch->blood);
}

void Projectile2::interact_Character2(Elements *tar){
    Character2* ch = dynamic_cast<Character2*>(tar);
    if(!ch) return;

    int dam = ch->blood - damage;
    ch->blood = (dam < 0) ? 0 : dam;

    printf("[Projectile] Character2 hit! HP = %d\n", ch->blood);
}

void Projectile2::Draw()
{
    if (!img || dele) return;

    /*
    if (hitbox) {
        Circle* c = static_cast<Circle*>(hitbox);
        al_draw_circle(c->x, c->y, c->r, al_map_rgb(255, 0, 0), 2);
    }*/

    int flags = (v > 0 ? ALLEGRO_FLIP_HORIZONTAL : 0);

    al_draw_bitmap(img, x, y, flags);
}
