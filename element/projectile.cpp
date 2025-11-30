#include "Projectile.h"
#include <allegro5/allegro.h>


#include "enemy.h"
#include "enemy2.h"
#include "enemy3.h"
#include "../shapes/Circle.h"
#include "../scene/gamescene.h"
#include "../scene/sceneManager.h"
#include "Character2.h"
#include "charater.h"
#include <stdio.h>
#include <stdlib.h>

// damage constants (same as你原本的 define)
static const int DAMAGE_V = 10;
static const int DAMAGE_L = 10;
static const int DAMAGE_K = 5;
static const int DAMAGE_C = 5;
static const int DAMAGE_J = 5;
static const int DAMAGE_X = 5;

Projectile::Projectile(int label_, int x_, int y_, int v_, void* owner_)
: label(label_), x(x_), y(y_), v(v_), owner(owner_), img(nullptr), hitbox(nullptr),
  element_wrapper(nullptr)
{
    const char* path = nullptr;
    switch (label) {
        case Projectile_K: path = "assets/image/projectile_k.png"; damage = DAMAGE_K; origin = 2; break;
        case Projectile_L: path = "assets/image/projectile_l.png"; damage = DAMAGE_L; origin = 2; break;
        case Projectile_J: path = "assets/image/projectile_j.png"; damage = DAMAGE_J; origin = 2; break;
        case Projectile_V: path = "assets/image/projectile_v.png"; damage = DAMAGE_V; origin = 1; break;
        case Projectile_C: path = "assets/image/projectile_c.png"; damage = DAMAGE_C; origin = 1; break;
        case Projectile_X: path = "assets/image/projectile_x.png"; damage = DAMAGE_X; origin = 1; break;
        default: path = nullptr; damage = 1; origin = 1; break;
    }

    if (path) img = al_load_bitmap(path);

    if (!img) {
        printf("[Projectile] ERROR: Failed to load image for label=%d, path='%s'\n", label, path ? path : "null");
    } else {
        width = al_get_bitmap_width(img);
        height = al_get_bitmap_height(img);
        printf("[Projectile] Loaded image for label=%d, path='%s', size=%d x %d\n",
               label, path, width, height);
    }

    if (!img) width = height = 16;

    // 建立圓形 hitbox
    hitbox = New_Circle(x + width/2, y + height/2, (min(width, height)/2));
}

Projectile::~Projectile()
{
    // element_wrapper 由 wrapper 的 Destroy 處理 (會 free)
    if (img) al_destroy_bitmap(img);
    
    if (hitbox) free(hitbox);
}

// Move and update hitbox
void Projectile::Update()
{   
    if (element_wrapper && element_wrapper->dele) {
    printf("[Projectile] Skipping action because dele=1 for label=%d\n", label);
    return;
    }
    printf("[DEBUG] Update called for label=%d, x=%d, y=%d, v=%d, dele=%d\n",
           label, x, y, v, element_wrapper ? element_wrapper->dele : -1);

    if (element_wrapper && element_wrapper->dele) {
        printf("[DEBUG] Skipping Update because dele=1 for label=%d\n", label);
        return;
    }

    x += v;
    // update hitbox: 你現有 Shape 的實作似乎是 function pointer style:
    if (hitbox && hitbox->update_center_x) {
        hitbox->update_center_x(hitbox, v);
        // vertical dy = 0 so no update_center_y call necessary unless needed
    }
  
}

// Interaction: scan labels that original projectiles used and apply effects.
// 因為我們在 wrapper 中把 pObj->inter_obj 填好，所以這邊為方便把交互放在 wrapper 的 Interact 呼叫中。
// 但如果你直接 call Projectile::Interact()，我們仍需使用全域 scene 和 _Get_label_elements。
void Projectile::Interact()
{   
    if (element_wrapper && element_wrapper->dele) {
        printf("[Projectile] (Interact)Skipping action because dele=1 for label=%d\n", label);
        return;
    }

    // 逐類別檢查（沿用你原本邏輯）

    // Enemy
   ElementVec enemies = _Get_label_elements(scene, Enemy_L);
   for (int i = 0; i < enemies.len; ++i) interactEnemy(enemies.arr[i]);

    // Enemy2
   ElementVec enemies2 = _Get_label_elements(scene, Enemy2_L);
   for (int i = 0; i < enemies2.len; ++i) interactEnemy2(enemies2.arr[i]);

    //Enemy3
    ElementVec enemies3 = _Get_label_elements(scene, Enemy3_L);
    for (int i = 0; i < enemies3.len; ++i) interactEnemy3(enemies3.arr[i]);

    
    
    
}

void Projectile::Draw()
{
    printf("[Projectile] Draw label=%d at (%d,%d)\n", label, x, y);
    if (element_wrapper && element_wrapper->dele) {
        printf("[Projectile] (Draw) Skipping action because dele=1 for label=%d\n", label);
        return;
    }

    if (!img) return;
    if (v > 0) al_draw_bitmap(img, x, y, ALLEGRO_FLIP_HORIZONTAL);
    else al_draw_bitmap(img, x, y, 0);
}


void Projectile::Destroy()
{
   
    
  

    printf("[Projectile] Destroy called for label=%d, x=%d, y=%d\n", label, x, y);
    if (element_wrapper) element_wrapper->dele = true; // 標記
    if (img) { al_destroy_bitmap(img); img = nullptr; }
    if (hitbox) { free(hitbox); hitbox = nullptr; }

}
/* ---------- interaction helpers ---------- */



void Projectile::interactEnemy(Elements* tar) {
    printf("[Projectile] (Projectile::interactEnemy) Interact called for label=%d at (%d,%d), dele=%d\n",
       label, x, y, element_wrapper ? element_wrapper->dele : -1);
    if (element_wrapper && element_wrapper->dele) return;
    element_wrapper->dele = true; // ✅ 立刻標記，防止下一行訪問 hitbox
    Enemy* enemy = (Enemy*)tar->pDerivedObj;
    if (origin == 2) {
        Character2* ch2 = (Character2*)owner;
        if (enemy->hitbox->overlap(enemy->hitbox, hitbox) && ch2->blood > 0 && enemy->hp > 0) {
            element_wrapper->dele = true;
            enemy->hp -= damage;
            if (enemy->hp <= 0) tar->dele = true;
            ch2->levelup_points++;
        }
    } else {
        Character* ch = (Character*)owner;
        if (enemy->hitbox->overlap(enemy->hitbox, hitbox) && ch->blood > 0 && enemy->hp > 0) {
            element_wrapper->dele = true;
            enemy->hp -= damage;
            if (enemy->hp <= 0) tar->dele = true;
            ch->levelup_points++;
        }
    }
}
void Projectile::interactEnemy2(Elements* tar) {
    printf("[Projectile] (Projectile::interactEnemy2) Interact called for label=%d at (%d,%d), dele=%d\n",
       label, x, y, element_wrapper ? element_wrapper->dele : -1);
    if (element_wrapper && element_wrapper->dele) return;
    element_wrapper->dele = true; // ✅ 立刻標記，防止下一行訪問 hitbox
    Enemy2* enemy = (Enemy2*)tar->pDerivedObj;
    if (origin == 2) {
        Character2* ch2 = (Character2*)owner;
        if (enemy->hitbox->overlap(enemy->hitbox, hitbox) && ch2->blood > 0 && enemy->hp > 0) {
            element_wrapper->dele = true;
            enemy->hp -= damage;
            if (enemy->hp <= 0) tar->dele = true;
            ch2->levelup_points++;
            return; // ✅ 立刻跳出，不要再訪問 hitbox
        }
    } else {
        Character* ch = (Character*)owner;
        if (enemy->hitbox->overlap(enemy->hitbox, hitbox) && ch->blood > 0 && enemy->hp > 0) {
            element_wrapper->dele = true;
            enemy->hp -= damage;
            if (enemy->hp <= 0) tar->dele = true;
            ch->levelup_points++;
            return; // ✅ 立刻跳出，不要再訪問 hitbox
        }
    }
}
void Projectile::interactEnemy3(Elements* tar) {
    printf("[Projectile] (Projectile::interactEnemy3) Interact called for label=%d at (%d,%d), dele=%d\n",
       label, x, y, element_wrapper ? element_wrapper->dele : -1);
    if (element_wrapper && element_wrapper->dele) return;
    element_wrapper->dele = true; // ✅ 立刻標記，防止下一行訪問 hitbox
    Enemy3* enemy = (Enemy3*)tar->pDerivedObj;
    if (origin == 2) {
        Character2* ch2 = (Character2*)owner;
        if (enemy->hitbox->overlap(enemy->hitbox, hitbox) && ch2->blood > 0 && enemy->hp > 0) {
            element_wrapper->dele = true;
            enemy->hp -= damage;
            if (enemy->hp <= 0) tar->dele = true;
            ch2->levelup_points++;
        }
    } else {
        Character* ch = (Character*)owner;
        if (enemy->hitbox->overlap(enemy->hitbox, hitbox) && ch->blood > 0 && enemy->hp > 0) {
            element_wrapper->dele = true;
            enemy->hp -= damage;
            if (enemy->hp <= 0) tar->dele = true;
            ch->levelup_points++;
        }
    }
}



/* ---------- wrapper: create Elements that forward to this object ---------- */

Elements* Projectile::toElements()
{
    
    
    if (element_wrapper) return element_wrapper;

    element_wrapper = New_Elements(label);
    // fill interaction labels same as original C implementation
 
    element_wrapper->inter_obj[element_wrapper->inter_len++] = Enemy_L;
    element_wrapper->inter_obj[element_wrapper->inter_len++] = Enemy2_L;
    element_wrapper->inter_obj[element_wrapper->inter_len++] = Enemy3_L;
    element_wrapper->inter_obj[element_wrapper->inter_len++] = Character_L;
    element_wrapper->inter_obj[element_wrapper->inter_len++] = Character2_L;

    element_wrapper->pDerivedObj = this; // store pointer to this object

    // assign C-style function pointers that call C++ methods
    element_wrapper->Update = [](Elements* e) {
        Projectile* p = (Projectile*)e->pDerivedObj;
        p->Update();
    };
    element_wrapper->Interact = [](Elements* e) {
        Projectile* p = (Projectile*)e->pDerivedObj;
        p->Interact();
    };
    element_wrapper->Draw = [](Elements* e) {
        Projectile* p = (Projectile*)e->pDerivedObj;
        p->Draw();
    };
    element_wrapper->Destroy = [](Elements* e) {
    Projectile* p = (Projectile*)e->pDerivedObj;
    if (p->element_wrapper) 
        p->element_wrapper->dele = true;  // ✅ 先標記 dele
    p->Destroy();  // 釋放 bitmap / hitbox
    delete p;      // 刪除 C++ 對象
    free(e);       // 刪除 wrapper
};

    // map internal numeric fields into element if needed (for other systems reading them)
    // e.g., store rendering pos in derived object? But we already keep p->x/p->y internally.

    return element_wrapper;
}

/* ---------- convenience C-style factory (跟你現有程式碼呼叫相同名稱) ---------- */

// 保留你原本程式碼呼叫方式： New_Projectile(label, x, y, v, owner)
Elements* New_Projectile(int label, int x, int y, int v, void* owner)
{
    Projectile* p = new Projectile(label, x, y, v, owner);
    Elements* e = p->toElements();
    return e;
}
