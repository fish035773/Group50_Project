#ifndef ENEMY3_H_INCLUDED
#define ENEMY3_H_INCLUDED


#include <allegro5/allegro_audio.h>
#include "element.h"
#include "../shapes/Shape.h"
#include "../algif5/algif.h"
#include <stdbool.h>


#define ENEMY3_MELEE_RANGE 200
#define ENEMY3_ATTACK_RANGE 300


typedef enum Enemy3State {
    ENEMY3_IDLE = 0,
    ENEMY3_MOVE,
    ENEMY3_ATK,
    ENEMY3_THROW,
    ENEMY3_DEAD,
} Enemy3State;




typedef struct _Enemy3 {
    int x, y;
    int width, height;
    bool dir;  // true: facing right, false: facing left
    Enemy3State state;
    ALGIF_ANIMATION *gif_status[5]; // stop, move, attack animations
    ALLEGRO_SAMPLE_INSTANCE *atk_sound;
    ALLEGRO_SAMPLE_INSTANCE *throw_sound;
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
    bool is_melee;        //melee attacks
} Enemy3;




Elements *New_Enemy3(int label);
void Enemy3_update(Elements *self);
void Enemy3_interact(Elements *self);
void Enemy3_draw(Elements *self);
void Enemy3_destroy(Elements *self);
void _Enemy3_update_position(Elements *self, int dx, int dy);
void enemy_charater3(int dx);

#endif



