#pragma once

#include <allegro5/allegro_audio.h>
#include "element.h"
#include "../shapes/Shape.h"
#include "../algif5/algif.h"

extern Elements* player;
extern Elements* player2;

#define ENEMY_ATTACK_RANGE 200

enum EnemyState {
    ENEMY_IDLE = 0,
    ENEMY_MOVE,
    ENEMY_ATK,
    ENEMY_DEAD
};

class Enemy : public Elements {
public:
    int x, y;
    int width, height;
    bool dir;
    EnemyState state;

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

    Enemy(int label, int speed);
    virtual ~Enemy();

    void Update() override;
    void Interact() override;
    void Draw() override;

    void update_position(int dx, int dy);
};
