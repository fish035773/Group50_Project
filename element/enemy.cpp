#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "enemy.h"
#include "projectile2.h"

#include "../scene/sceneManager.h"
#include "../shapes/Rectangle.h"
#include "../algif5/algif.h"
#include "../scene/gamescene.h"
#include "../element/charater.h"
#include "../element/projectile2.h"
#define maxhp 50
/*
   [Character function]
*/

Enemy::Enemy(int label): Elements(label){
    char state_string[4][10] = {"stop", "move", "attack", "dead"};
    for (int i = 0; i < 4; i++){
        char buffer[100];
        sprintf(buffer, "assets/image/enemy_%s.gif", state_string[i]);
        gif_status[i] = algif_load_animation(buffer);
    }

    ALLEGRO_SAMPLE *sample = al_load_sample("assets/sound/fire_atk_sound.mp3");
    atk_sound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(atk_sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(atk_sound, al_get_default_mixer());

    width = gif_status[0]->width;
    height = gif_status[0]->height;

    x = 400;
    y = HEIGHT - height - 60;

    hitbox = new Rectangle(
        x, y,
        x + width,
        y + height
    );

    dir = true;
    state = ENEMY_IDLE;
    active_proj = false;
    move_counter = 0;

    hp = maxhp;
    alive = true;
    got_hit = false;
    hit_time = 0;
    dying = false;
    death_time = 0;

    spawn_time = al_get_time();
    can_attack = false;
    chasing = false;
}

Enemy::~Enemy(){
    al_destroy_sample_instance(atk_sound);
    for (int i = 0; i < 4; i++)
        algif_destroy_animation(gif_status[i]);

    delete hitbox;
}

void Enemy::update_position(int dx, int dy){
    x += dx;
    y += dy;

    hitbox->update_center_x(dx);
    hitbox->update_center_y(dy);
}

int player_center_x1;
void enemy_charater(int dx){
    player_center_x1 = dx;
}

void Enemy::Update(){
    extern Elements *player;
    
    if(hp < 0) return;
    int speed = 2;

    int enemy_center_x = x + width / 2;
    int dx = player_center_x1 - enemy_center_x;

    double current_time = al_get_time();
    if (!can_attack && current_time - spawn_time >= 1.0) {
        can_attack = true; // allow attacking after 1 second
    }

    if (can_attack && abs(dx) <= ENEMY_ATTACK_RANGE){
        dir = dx >= 0;

        // If not already attacking, switch to attack
        if (state != ENEMY_ATK){   
            state = ENEMY_ATK;
            active_proj = false;
            move_counter = 0;
        }
    }

    if (state == ENEMY_DEAD) {
        if (death_time == 0) death_time = al_get_time(); // mark when death started
        else if (gif_status[ENEMY_DEAD]->done && al_get_time() - death_time >= 1.0) // wait 1 second
            dele = true;
        return;
    }else if (state == ENEMY_IDLE){
        if (abs(dx) <= ENEMY_ATTACK_RANGE){
            chasing = true;
            state = ENEMY_ATK;
            active_proj = false;
            move_counter = 0;
        }else{
            state = ENEMY_MOVE;
        }
    }else if (state == ENEMY_MOVE){
        if (chasing) {
            // Chase player regardless of patrol area
            dir = (dx >= 0);
            int move_speed = (dx > 0) ? speed : -speed;
            update_position(move_speed, 0);
        }else {
            // Normal patrol between 400 and 700
            if (!dir) {
                update_position(-speed, 0);
                if (x < 400) dir = true;
            } else {
                update_position(speed, 0);
                if (x > 700) dir = false;
            }
        }  
    }else if (state == ENEMY_ATK){
        int current_frame = gif_status[ENEMY_ATK]->display_index;

        // Launch projectile once at frame 2
        if (current_frame == 2 && !active_proj){
            int projectile_x = dir
                ? x + width - 100
                : x + 20;

            Elements* pro = New_Projectile2(
                Projectile2_L,
                projectile_x,
                y + height / 2 - 20,
                dir ? 5 : -5
            );

            //===========TODO===============
            ((Projectile2 *)pro->pDerivedObj)->is_enemy_projectile = true;
            _Register_elements(scene, pro);
            //====

            active_proj = true;
        }

        // When attack animation finishes, transition based on distance
        if (gif_status[ENEMY_ATK]->done) {
            gif_status[ENEMY_ATK]->done = false;
            gif_status[ENEMY_ATK]->display_index = 0;
            active_proj = false;

            if (std::abs(dx) <= ENEMY_ATTACK_RANGE) {
                chasing = true;
                state = ENEMY_ATK;
            }
            else {
                chasing = true;
                state = ENEMY_MOVE;
            }
        }
    }
}

void Enemy::Draw(){
    if(hp <= 0) return;
    
    ALLEGRO_BITMAP *frame = algif_get_bitmap(gif_status[state], al_get_time());
    if(!frame) return;

    if (frame){
        al_draw_bitmap(frame, x, y, (dir ? 0 : ALLEGRO_FLIP_HORIZONTAL));
    }
    if (state == ENEMY_ATK && gif_status[state]->display_index == 2){
        al_play_sample_instance(atk_sound);
    }
    
    // Draw the red HP bar
    int bar_width = 100;
    int bar_height = 10;
    int bar_x = x + 80;
    int bar_y = y; // position above the enemy

    float hp_ratio = (float)hp / maxhp; // max HP = 3, change if needed

    al_draw_filled_rectangle(bar_x, bar_y, bar_x + (int)(bar_width * 1), bar_y + bar_height, al_map_rgb(225, 225, 225));
    al_draw_filled_rectangle(bar_x, bar_y, bar_x + (int)(bar_width * hp_ratio), bar_y + bar_height, al_map_rgb(150, 0, 0));
   
    if (got_hit && al_get_time() - hit_time < 0.2) {
        al_draw_tinted_bitmap(frame, al_map_rgba(255, 0, 0, 200), x, y, (dir ? 0 : ALLEGRO_FLIP_HORIZONTAL));
    } else {
        al_draw_bitmap(frame, x, y, (dir ? 0 : ALLEGRO_FLIP_HORIZONTAL));
    }
}

void Enemy::Interact()
{
    if (!alive || dying) return;

    for (int i = 0; i < MAX_ELEMENT; i++) {
        EPNode* node = scene->ele_list[i];
        while (node) {
            Elements* obj = node->ele;
            EPNode* next = node->next;

            // hit by projectile
            if (obj->label == Projectile2_Right ||
                obj->label == Projectile2_Left ||
                obj->label == Projectile2_L)
            {
                Projectile2* proj = (Projectile2*)obj->pDerivedObj;
                Shape* proj_hitbox = proj->hitbox;

                if (proj->is_enemy_projectile) {
                    node = next;
                    continue;
                }

                if (hitbox->overlap(*proj_hitbox)) {

                    obj->dele = true;

                    hp--;
                    got_hit = true;
                    hit_time = al_get_time();

                    if (hp <= 0 && !dying) {
                        alive = false;
                        dying = true;
                        state = ENEMY_DEAD;
                        gif_status[ENEMY_DEAD]->display_index = 0;
                        gif_status[ENEMY_DEAD]->done = false;
                        death_time = al_get_time();
                        return;
                    }
                }
            }
            node = next;
        }
    }
}

Elements* New_Enemy(int label)
{
    return new Enemy(label);
}