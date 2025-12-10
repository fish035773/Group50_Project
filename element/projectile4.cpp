#include "projectile4.h"
#include "charater.h"
#include "Character2.h"
#include "enemy.h"
#include "../shapes/Circle.h"
#include "../scene/gamescene.h"
#include "../scene/sceneManager.h"

Projectile4::Projectile4(int label, int x, int y, int v)
    : Elements(label), x(x), y(y), v(v)
{

    img = al_load_bitmap("assets/image/projectile3.png");
    damage = 5;
    v = 0;
    falling = false;

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

Projectile4::~Projectile4() {
    if (img) al_destroy_bitmap(img);
    if (hitbox) delete hitbox;
}

void Projectile4::Update() {
    // 垂直落下
    update_position(0, v);

    // 超出畫面就刪除
    if (y > HEIGHT + height)
        dele = true;
}

void Projectile4::update_position(int dx, int dy) {
    x += dx;
    y += dy;
    hitbox->update_position(dx, dy);
}

void Projectile4::Interact() {
    if (dele || !scene) return;

    for (Elements* ele : scene->getAllElements()) {
        if (ele->dele) continue;
        if (ele->label != Character_L && ele->label != Character2_L) continue;

        if (Character* c1 = dynamic_cast<Character*>(ele)) {
            if (hitbox->overlap(*c1->hitbox)) {
                dele = true;
                interact_Character(c1);
                return;
            }
        }
        if (Character2* c2 = dynamic_cast<Character2*>(ele)) {
            if (hitbox->overlap(*c2->hitbox)) {
                dele = true;
                interact_Character2(c2);
                return;
            }
        }
    }
}

void Projectile4::interact_Floor(Elements *tar) {}

void Projectile4::interact_Character(Elements *tar){
    Character* ch = dynamic_cast<Character*>(tar);
    if(!ch) return;

    ch->blood = std::max(0, ch->blood - damage);
    printf("[Projectile4] Character1 hit! HP = %d\n", ch->blood);
}

void Projectile4::interact_Character2(Elements *tar){
    Character2* ch = dynamic_cast<Character2*>(tar);
    if(!ch) return;

    ch->blood = std::max(0, ch->blood - damage);
    printf("[Projectile4] Character2 hit! HP = %d\n", ch->blood);
}

void Projectile4::Draw() {
    al_draw_bitmap(img, x, y, 0);
}
