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
    if(label == Projectile3_1_L){
        img = al_load_bitmap("assets/image/projectile3.png");
        damage = 5;
    }else if(label == Projectile3_2_L){
        img = al_load_bitmap("assets/image/projectile2.png");
        damage = 8;
    }

    if (!img) {
        width = height = 16;
    } else {
        width = al_get_bitmap_width(img);
        height = al_get_bitmap_height(img);
    }

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

void Projectile3::interact_Floor(Elements *tar) {
    if (x < -width)
        dele = true;
    else if (x > WIDTH + width)
        dele = true;
}

void Projectile3::interact_Character(Elements *tar){
    Character* ch = dynamic_cast<Character*>(tar);
    if(!ch) return;

    ch->blood -= damage;
    
    printf("[Projectile] Character1 hit! HP = %d\n", ch->blood);
}

void Projectile3::interact_Character2(Elements *tar){
    Character2* ch = dynamic_cast<Character2*>(tar);
    if(!ch) return;

    ch->blood -= damage;

    printf("[Projectile] Character2 hit! HP = %d\n", ch->blood);
}

void Projectile3::Draw() {
    if (v < 0)
        al_draw_bitmap(img, x, y, ALLEGRO_FLIP_HORIZONTAL);
    else
        al_draw_bitmap(img, x, y, 0);
}

