#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "../shapes/Shape.h"
#include "../element/element.h"   // Elements, labels
#include "../scene/sceneManager.h"
#include "../scene/gamescene.h"

class Projectile {
public:
    // labels should match your enum/defines (Projectile_X, etc.)
    Projectile(int label, int x, int y, int v, void* owner);
    ~Projectile();
    enum ProjectileType { Projectile_X, Projectile_C, Projectile_V, Projectile_K, Projectile_L, Projectile_J };
    int x, y;
    int width, height;
    ALLEGRO_BITMAP* img;
    // core behaviour (call from wrapper function)
    void Update();       // 移動、更新 hitbox
    void Interact();     // 與場上物件互動（會使用全域 scene）
    void Draw();         // 繪製
    void Destroy();      // 釋放資源（也由 wrapper 呼叫）

    // accessors for wrapper if needed
    Elements* toElements(); // 建立一個 Elements wrapper（只建立一次，回傳同一個 pointer）

private:
    // helpers for interactions
    void interactTree(Elements* tar);
    void interactFloor(Elements* tar);
    void interactEnemy(Elements* tar);
    void interactEnemy2(Elements* tar);
    void interactEnemy3(Elements* tar);
    void interactCharacter(Elements* tar);
    void interactCharacter2(Elements* tar);

private:
    int label;          // Projectile label (Projectile_X ...)
           
    
    int v;              // velocity (x direction)
    int damage;
    int origin;         // 1 => from Character1, 2 => from Character2
    void* owner;        // pointer to owner instance (Character* or Character2*)
   
    Shape* hitbox;

    // cached wrapper Elements (so New_Projectile can return it)
    Elements* element_wrapper;
};
// Wrapper function to create a new Projectile and return as Elements*
Elements* New_Projectile(int label, int x, int y, int v, void* owner);