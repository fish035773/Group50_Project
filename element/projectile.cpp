#include "Projectile.h"
#include <allegro5/allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include "../scene/sceneManager.h" 
#include "enemy.h"
#include "enemy2.h"
#include "enemy3.h"
#include "../shapes/Circle.h"
#include "../scene/gamescene.h"
#include "../scene/scene.h"
#include "Character2.h"
#include "charater.h"
#include <allegro5/allegro_primitives.h>

// Damage constants

static const int DAMAGE_V = 10;
static const int DAMAGE_L = 10;
static const int DAMAGE_K = 5;
static const int DAMAGE_C = 5;
static const int DAMAGE_J = 5;
static const int DAMAGE_X = 5;

/*
static const int DAMAGE_V = 100;
static const int DAMAGE_L = 100;
static const int DAMAGE_K = 500;
static const int DAMAGE_C = 500;
static const int DAMAGE_J = 500;
static const int DAMAGE_X = 500;
*/

Projectile::Projectile(int label_, int x_, int y_, int v_, void* owner_)
    : Elements(label_), x(x_), y(y_), width(0), height(0),
      img(nullptr), v(v_), damage(0), origin(0),
      owner(owner_), hitbox(nullptr)
{
    const char* path = nullptr;

    switch (label_) {
        case Projectile_K: path = "assets/image/projectile_k.png"; damage = DAMAGE_K + ((Character2*)owner)->skill2_damage; origin = 2; break;
        case Projectile_L: path = "assets/image/projectile_l.png"; damage = DAMAGE_L + ((Character2*)owner)->skill2_damage; origin = 2; break;
        case Projectile_J: path = "assets/image/projectile_j.png"; damage = DAMAGE_J + ((Character2*)owner)->skill2_damage; origin = 2; break;
        case Projectile_V: path = "assets/image/projectile_v.png"; damage = DAMAGE_V + ((Character*)owner)->skill2_damage; origin = 1; break;
        case Projectile_C: path = "assets/image/projectile_c.png"; damage = DAMAGE_C + ((Character*)owner)->skill2_damage; origin = 1; break;
        case Projectile_X: path = "assets/image/projectile_x.png"; damage = DAMAGE_X + ((Character*)owner)->skill2_damage; origin = 1; break;
    }

    //printf("[Projectile] Creating projectile label=%d at (%d,%d)\n", label_, x_, y_);

    if (path)
        img = al_load_bitmap(path);
    else printf("[Projectile] ERROR loading image for projectile label=%d\n", label_);

    if (!img) {
        //printf("[Projectile] ERROR loading image for projectile label=%d\n", label_);
        width = height = 16;
    } else {
        width = al_get_bitmap_width(img);
        height = al_get_bitmap_height(img);
        //printf("[Projectile] Loaded image size = %d x %d\n", width, height);
    }

    // hitbox
    hitbox = new Circle(x + width / 2, y + height / 2, (std::min(width, height) / 2));
}

Projectile::~Projectile()
{
    if (img)
        al_destroy_bitmap(img);
    if (hitbox)
        delete hitbox;

    //printf("[Projectile] Destroyed projectile label=%d\n", label);
}

void Projectile::Update()
{
    if (dele) return;
    x += v;
    
    if (hitbox)
        hitbox->update_position(v, 0);

    if (x < 0 || x > 900) {
        dele = true;
        //printf("[Projectile] Out of bounds → dele set to true\n");
    }else {
        //Circle* c = static_cast<Circle*>(hitbox);
        //printf("X: %d, %lf Y: %d, %lf\n", x, c->center_x(), y, c->center_y());
    }
}

void Projectile::Interact()
{
    if (dele || !scene) return;

    for (Elements* obj : scene->getAllElements()) {
        if (obj->dele) continue;
        if (obj == owner) continue;

        Enemy* e1  = dynamic_cast<Enemy*>(obj);
        Enemy2* e2 = dynamic_cast<Enemy2*>(obj);
        Enemy3* e3 = dynamic_cast<Enemy3*>(obj);

        if (!e1 && !e2 && !e3)
            continue;

        Shape* enemy_hitbox =
            e1 ? e1->hitbox :
            e2 ? e2->hitbox :
            e3 ? e3->hitbox : nullptr;

        if (!enemy_hitbox)
            continue;

        if (hitbox->overlap(*enemy_hitbox)) {
            dele = true;
            if (god) damage += 1000;
            if (e1) interactEnemy(e1);
            if (e2) interactEnemy2(e2);
            if (e3) interactEnemy3(e3);

            return;
        }
    }
}

void Projectile::Draw()
{
    if (dele) return;
    
    if (hitbox) {
        //Circle* c = static_cast<Circle*>(hitbox);
        //al_draw_circle(c->x, c->y, c->r, al_map_rgb(255, 0, 0), 2);
    }

    int flags = (v > 0 ? ALLEGRO_FLIP_HORIZONTAL : 0);

    al_draw_bitmap(img, x, y, flags);
    //al_draw_rectangle(x, y, x+width, y+height, al_map_rgb(0,255,0), 2);
}

void Projectile::interactEnemy2(Elements* tar)
{
    Enemy2* enemy = dynamic_cast<Enemy2*>(tar);
    if (!enemy) return;

    enemy->hp -= damage;

    bool kill = (enemy->hp <= 0);
    if (kill) tar->dele = true;

    // 判斷 owner 是誰
    

    if (origin == 2) ((Character2*)owner)->OnHitEnemy(damage, kill);
    else  ((Character*)owner)->OnHitEnemy(damage, kill);
}
void Projectile::interactEnemy3(Elements* tar)
{
    Enemy3* enemy = dynamic_cast<Enemy3*>(tar);
    if (!enemy) return;

    enemy->hp -= damage;

    bool kill = (enemy->hp <= 0);
    if (kill) tar->dele = true;

    // 判斷 owner 是誰
    

    if (origin == 2) ((Character2*)owner)->OnHitEnemy(damage, kill);
    else  ((Character*)owner)->OnHitEnemy(damage, kill);
}

void Projectile::interactEnemy(Elements* tar)
{
    Enemy* enemy = dynamic_cast<Enemy*>(tar);
    if (!enemy) return;

    if (enemy->hp > 0)
        enemy->hp -= damage;
    bool kill = (enemy->hp <= 0);
    if (kill) tar->dele = true;
    // award owner EXP
   if (origin == 2) ((Character2*)owner)->OnHitEnemy(damage, kill);
    else  ((Character*)owner)->OnHitEnemy(damage, kill);
    printf("[Projectile] Enemy1 hit! HP = %d\n", enemy->hp);
}
