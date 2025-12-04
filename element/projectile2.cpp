#include "projectile2.h"
#include "charater.h"
#include "Character2.h"
#include "enemy.h"

#include "../shapes/Circle.h"
#include "../scene/sceneManager.h"
#include "../scene/gamescene.h"
#include "element.h"

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
}

void Projectile2::Update() {
    x += v;
    hitbox->update_position(v, 0);

    if (x < -width || x > WIDTH + width)
        dele = true;
}

// ----------------------------------------
// Interact: C++ 版本正確 hitbox 判定
// ----------------------------------------
void Projectile2::Interact()
{
    if (dele || !scene) return;

    for (Elements* ele : scene->getAllElements()) {

        if (ele->dele) continue;
        if (ele == this) continue;

        // ----- 玩家 1 -----
        if (Character* c1 = dynamic_cast<Character*>(ele)) {

            // 如果這是敵人射的，才打玩家
            if (is_enemy_projectile &&
                hitbox->overlap(*c1->hitbox))
            {
                c1->blood -= damage;
                dele = true;
                return;
            }

            continue;
        }

        // ----- 玩家 2 -----
        if (Character2* c2 = dynamic_cast<Character2*>(ele)) {

            if (is_enemy_projectile &&
                hitbox->overlap(*c2->hitbox))
            {
                c2->blood -= damage;
                dele = true;
                return;
            }

            continue;
        }

        if (Enemy* e = dynamic_cast<Enemy*>(ele)) {

            // 玩家射的子彈才能打敵人
            if (!is_enemy_projectile &&
                hitbox->overlap(*e->hitbox))
            {
                e->hp -= damage;
                dele = true;
                return;
            }

            continue;
        }
    }
}

// ----------------------------------------
// Draw
// ----------------------------------------
void Projectile2::Draw()
{
    if (!img || dele) return;

    if (v < 0)
        al_draw_bitmap(img, x, y, ALLEGRO_FLIP_HORIZONTAL);
    else
        al_draw_bitmap(img, x, y, 0);
}
