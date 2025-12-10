#pragma once

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <vector>
#include "element.h"
#include "../shapes/Shape.h"
#include "../algif5/algif.h"

extern Elements* player;
extern Elements* player2;

#define BOSS_ATTACK_RANGE 200

enum BossState {
    BOSS_IDLE = 0,
    BOSS_MOVE,
    BOSS_ATK,
    BOSS_DEAD
};

class Boss : public Elements {
public:
    int x, y;
    int width, height;
    bool dir;
    BossState state;

    ALGIF_ANIMATION* gif_status[4];
    ALLEGRO_SAMPLE_INSTANCE* atk_sound;

    int anime;
    int anime_time;
    Shape* hitbox;
    bool active_proj;
    int move_counter;

    int hp;
    bool alive;
    bool got_hit;
    double hit_time;
    bool dying;

    double spawn_time;
    bool can_attack;
    bool chasing;
    double death_time;
    int speed;

    double last_attack_time;
    double attack_interval;

    double scale;
    
    //QTE
    bool qte_active;
    double qte_start_time;
    double qte_duration;   // 玩家允許反應時間 
    double next_qte_time;
    
    std::vector<int> qte_keys; // 要按的按鍵（keycode）
    int qte_index;             // 玩家要按的順序

    ALLEGRO_FONT* font;
    int text_size;

    Boss(int label, int speed);
    virtual ~Boss();

    void Update() override;
    void Interact() override;
    void Draw() override;

    void update_position(int dx, int dy);
    void SpawnFallingAttack();
    void StartQTE();
    void PlayerTakeDamage(int dmg);
    void HandleQTEKey(int keycode);
};
