#pragma once

#include <allegro5/allegro_audio.h>
#include "element.h"
#include "../shapes/Shape.h"
#include "../algif5/algif.h"

extern Elements* player;
extern Elements* player2;

#define ENEMY_ATTACK_RANGE 300

enum Enemy2State {
    ENEMY2_IDLE = 0,
    ENEMY2_MOVE,
    ENEMY2_ATK,
    ENEMY2_DEAD
};

class Enemy2 : public Elements {
public:
    int x, y;
    int width, height;
    bool dir;
    Enemy2State state;

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

    Enemy2(int label);
    virtual ~Enemy2();

    void Update() override;
    void Interact() override;
    void Draw() override;

    void update_position(int dx, int dy);
};

void enemy_charater(int dx);






















