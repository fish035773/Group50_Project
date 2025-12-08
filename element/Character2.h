#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "../shapes/Shape.h"
#include "../algif5/algif.h"
#include "projectile.h"
// --- 必須加這三行 ---
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
class Character2: public Elements
{
public:
    enum State2 { STOP = 0, MOVE, ATK };
    enum ProjectileType { Projectile_X, Projectile_C, Projectile_V, Projectile_K, Projectile_L, Projectile_J };

    // 建構與解構
   // 建構子 / 解構子
    Character2();
    ~Character2();
    
    void Update() override;
    void Draw() override;
    void Interact() override;
    void update_position(int dx, int dy);
   void OnHitEnemy(int damage, bool kill);
    // 成員變數
    int x, y;
    int width, height;
    int blood;
    int level;
    int levelup_points;
    int add_blood;
   int skill_3_add_blood = 0;
    bool dir; // false: left, true: right
    bool is_jumping;
    double jump_start_time;
    int ground_y;

    int cool_J, cool_K, cool_L;
    int atk_type;
    bool new_proj;
    State2 state;
    bool died;
    int skill2_damage = 0;
    Shape* hitbox;
    ALGIF_ANIMATION* gif_status[3];
    ALLEGRO_SAMPLE_INSTANCE* atk_Sound;
    // ===== UI Font =====
    ALLEGRO_FONT* font;
    int coins2 = 0;
    // ===== Background =====
   ALLEGRO_BITMAP* Skill[4] = {nullptr};
   

private:
   void trigger_attack(int atk_type);
};


