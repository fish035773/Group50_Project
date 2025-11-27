#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED




#include <allegro5/allegro_audio.h>
#include "element.h"
#include "../shapes/Shape.h"
#include "../algif5/algif.h"
#include <stdbool.h>

extern Elements * player;

extern Elements * player2;

#define ENEMY_ATTACK_RANGE 300




typedef enum EnemyState {
    ENEMY_IDLE = 0,
    ENEMY_MOVE,
    ENEMY_ATK,
    ENEMY_DEAD
} EnemyState;




typedef struct _Enemy {
    int x, y;
    int width, height;
    bool dir;  // true: facing right, false: facing left
    EnemyState state;
    ALGIF_ANIMATION *gif_status[4]; // stop, move, attack animations
    ALLEGRO_SAMPLE_INSTANCE *atk_sound;
    int anime;
    int anime_time;
    Shape *hitbox;
    bool active_proj; // to control attack projectiles spawn
    int move_counter;


    int hp;
    bool alive;
    bool got_hit;
    double hit_time;          // for flash effect
    bool dying;               // started death animation

   


    double spawn_time;    // time the enemy spawned
    bool can_attack;      // flag to indicate if attack check is allowed
    bool chasing;
    double death_time;
} Enemy;




Elements *New_Enemy(int label);
void Enemy_update(Elements *self);
void Enemy_interact(Elements *self);
void Enemy_draw(Elements *self);
void Enemy_destroy(Elements *self);
void _Enemy_update_position(Elements *self, int dx, int dy);
void enemy_charater(int dx);



#endif









