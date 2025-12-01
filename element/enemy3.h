#pragma once

#include <allegro5/allegro_audio.h>
#include "element.h"
#include "../shapes/Shape.h"
#include "../algif5/algif.h"

extern Elements* player;
extern Elements* player2;

#define ENEMY3_ATTACK_RANGE 300
#define ENEMY3_MELEE_RANGE 200
enum Enemy3State {
    ENEMY3_IDLE = 0,
    ENEMY3_MOVE,
    ENEMY3_ATK,
    ENEMY3_THROW,
    ENEMY3_DEAD
};

class Enemy3 : public Elements {
public:
    int x, y;
    int width, height;
    bool dir;
    Enemy3State state;

    ALGIF_ANIMATION* gif_status[5];
    ALLEGRO_SAMPLE_INSTANCE* atk_sound;
    ALLEGRO_SAMPLE_INSTANCE* throw_sound;

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

    double spawn_time;    // time the enemy spawned
    bool can_attack;      // flag to indicate if attack check is allowed
    bool chasing;
    double death_time;
    bool is_melee;        //melee attacks

    Enemy3(int label);
    virtual ~Enemy3();

    void Update() override;
    void Interact() override;
    void Draw() override;

    void update_position(int dx, int dy);
};

void enemy_charater(int dx);
Elements* New_Enemy3(int label);



