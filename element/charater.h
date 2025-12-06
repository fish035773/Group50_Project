#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "../shapes/Shape.h"
#include "../algif5/algif.h"
#include "projectile.h"
class Character : public Elements{
public:
    enum State { STOP = 0, MOVE, ATK1 };
    enum ProjectileType { Projectile_X, Projectile_C, Projectile_V, Projectile_K, Projectile_L, Projectile_J };


    // 屬性
    double x, y;
    int width, height;
    int blood;
    int level;
    int levelup_points;
    int add_blood;

    bool dir;         // false = left, true = right
    bool is_jumping;
    double jump_start_time;
    double ground_y;

    int cool_X, cool_C, cool_V;
    int atk_type;
    bool new_proj;

    State state;
    
    // 資源
    ALGIF_ANIMATION* gif_status[3];   // 修正型別
    ALLEGRO_SAMPLE_INSTANCE* atk_Sound;
    Shape* hitbox;

    // 建構子 / 解構子
    Character();
    ~Character();

    // 成員函式
    void Update() override;
    void Draw() override;
    void Interact() override;

    void update_position(int dx, int dy);
private:
    void trigger_attack(int atk_type);
   
};
