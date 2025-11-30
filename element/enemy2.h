#ifndef ENEMY2_H_INCLUDED
#define ENEMY2_H_INCLUDED


#include <allegro5/allegro_audio.h>
#include "element.h"
#include "../shapes/Shape.h"
#include "../algif5/algif.h"
#include <stdbool.h>


#define ENEMY2_ATTACK_RANGE 150


typedef enum Enemy2State {
    ENEMY2_IDLE = 0,
    ENEMY2_MOVE,
    ENEMY2_ATK,
    ENEMY2_DEAD
} Enemy2State;




typedef struct _Enemy2 {
    int x, y;
    int width, height;
    bool dir;  // true: facing right, false: facing left
    Enemy2State state;
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
} Enemy2;




Elements *New_Enemy2(int label);
void Enemy2_update(Elements *self);
void Enemy2_interact(Elements *self);
void Enemy2_draw(Elements *self);
void Enemy2_destroy(Elements *self);
void _Enemy2_update_position(Elements *self, int dx, int dy);
void enemy2_charater(int chra2x);



#endif






















