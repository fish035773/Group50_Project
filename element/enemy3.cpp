#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "enemy3.h"
#include "projectile3.h"
#include "../scene/sceneManager.h"
#include "../shapes/Rectangle.h"
#include "../algif5/algif.h"
#include "../scene/gamescene.h"
#include "charater.h"
#include "Character2.h"
#include "../element/projectile3.h"
#include <stdio.h>
#include <stdbool.h>
#define maxhp 100

Enemy3::Enemy3(int label, int speed): Elements(label), speed(speed){
    char state_string[5][10] = {"stop", "move", "attack", "throw", "dead"};
    for (int i = 0; i < 5; i++){
        char buffer[100];
        sprintf(buffer, "assets/image/enemy3_%s.gif", state_string[i]);
        gif_status[i] = algif_load_animation(buffer);
    }
    // Load sound
    ALLEGRO_SAMPLE *atk_sample = al_load_sample("assets/sound/hit_sound.mp3");
    atk_sound = al_create_sample_instance(atk_sample);
    al_set_sample_instance_playmode(atk_sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(atk_sound, al_get_default_mixer());


    ALLEGRO_SAMPLE *throw_sample = al_load_sample("assets/sound/throw_sound.mp3");
    throw_sound = al_create_sample_instance(throw_sample);
    al_set_sample_instance_playmode(throw_sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(throw_sound, al_get_default_mixer());;

    width = gif_status[0]->width;
    height = gif_status[0]->height;

    x = 200;
    y = HEIGHT - width + 20;

    hitbox = new Rectangle(
        x * 1.3, y * 1.0,
        x + width * 0.8,
        y + height * 0.9
    );

    dir = true;
    state = ENEMY3_IDLE;
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

Enemy3::~Enemy3() {
    if (hitbox) delete hitbox;

    for (int i = 0; i < 4; i++) {
        if (gif_status[i])
            algif_destroy_animation(gif_status[i]);
    }

    if (atk_sound) al_destroy_sample_instance(atk_sound);
}

void Enemy3::update_position(int dx, int dy){
    //printf("UPDATED\n");
    x += dx;
    y += dy;

    hitbox->update_position(dx, dy);
}

void Enemy3::Update() {
    //Rectangle* b = (Rectangle*)dynamic_cast<Rectangle*>(hitbox);
    //printf("%d %d\n", x, b->get_pos_x());
    if (hp <= 0) return;
    //printf("%d\n", x);
    //int c1_pos = INT_MAX, c2_pos = INT_MAX;
    //int nearest_player_center = 0;
    int enemy_center_x = x + width / 2;

    Elements* target = nullptr;
    int bestDist = INT_MAX;

    for (Elements* ele : scene->getAllElements()) {
        if (ele->label == Character_L || ele->label == Character2_L) {

            int px = 0;

            if (ele->label == Character_L) {
                Character* c = dynamic_cast<Character*>(ele);
                if (!c) continue;
                px = c->x + c->width / 2;
            }
            else {
                Character2* c2 = dynamic_cast<Character2*>(ele);
                if (!c2) continue;
                px = c2->x + c2->width / 2;
            }

            int dist = abs(px - enemy_center_x);
            if (dist < bestDist) {
                bestDist = dist;
                target = ele;
            }
        }
    }

    if (!target) return;

    int player_center_x = 0;

    if (target->label == Character_L) {
        Character* c = dynamic_cast<Character*>(target);
        player_center_x = c->x + c->width / 2;
    } else {
        Character2* c2 = dynamic_cast<Character2*>(target);
        player_center_x = c2->x + c2->width / 2;
    }

    int dx = player_center_x - enemy_center_x;
    //printf("DX: %d\n", dx);
    double current_time = al_get_time();
    
    if (!can_attack && current_time - spawn_time >= 1.0)
        can_attack = true;

    if (can_attack && abs(dx) <= ENEMY3_ATTACK_RANGE) {
        dir = dx >= 0;

        if (state != ENEMY3_ATK) {
            state = ENEMY3_ATK;
            active_proj = false;
            al_play_sample_instance(atk_sound);
        }
    }
 
    switch (state) {
        // ----------------- IDLE -----------------
        case ENEMY3_IDLE:
            //printf("IDLE\n");
            if (abs(dx) <= ENEMY3_MELEE_RANGE && can_attack) {
                dir = dx >= 0;
                state = ENEMY3_ATK;    // 近距離 = 近戰
                active_proj = false;
            }
            else if (abs(dx) <= ENEMY3_ATTACK_RANGE && can_attack) {
                dir = dx >= 0;
                state = ENEMY3_THROW;  // 中距離 = 投擲
                active_proj = false;
            }
            else {
                state = ENEMY3_MOVE;   // 否則 = 追擊
            }
            break;
        // ----------------- MOVE -----------------
        case ENEMY3_MOVE:
            //printf("MOVE\n");
            if (abs(dx) <= ENEMY3_ATTACK_RANGE && can_attack) {
                //printf("1\n");
                dir = dx >= 0;
                state = ENEMY3_ATK;
                active_proj = false;
                al_play_sample_instance(atk_sound);
            } else if(abs(dx) <= ENEMY3_MELEE_RANGE && can_attack){
                //printf("2\n");
                dir = dx >= 0;
                state = ENEMY3_THROW;
                active_proj = false;
                al_play_sample_instance(atk_sound);
            }else {
                //printf("3\n");
                if (chasing) {
                    //printf("CH\n");
                    dir = (dx >= 0);
                    int move_speed = (dx > 0) ? speed : -speed;
                    update_position(move_speed, 0);
                } else {
                    if (!dir) {
                        //printf("CH2\n");
                        update_position(-speed, 0);
                        if (x < 200) dir = true;
                    } else {
                        //printf("%d\n", speed);
                        update_position(speed, 0);
                        if (x > 800) dir = false;
                    }
                }
            }
            break;
        case ENEMY3_ATK: {
            //printf("ATK\n");
            int frame = gif_status[ENEMY3_ATK]->display_index;
            int last = gif_status[ENEMY3_ATK]->frames_count - 1;

            if (frame == 2 && !active_proj) {
                int px = dir ? (x + width - 100) : (x + 100);

                Elements* pro = new Projectile3(
                    Projectile3_1_L,
                    px,
                    y + height / 2 - 20,
                    dir ? 5 : -5
                );

                Projectile3* p = dynamic_cast<Projectile3*>(pro);
                if (p) p->is_enemy_projectile = true;
                scene->addElement(p);

                al_play_sample_instance(atk_sound);
                active_proj = true;
            }

            if (frame == last) {
                gif_status[ENEMY3_ATK]->display_index = 0;
                active_proj = false;

                if (abs(dx) <= ENEMY3_ATTACK_RANGE)
                    state = ENEMY3_IDLE;
                else state = ENEMY3_MOVE;
            }
            break;
        }
        // ----------------- THROW -----------------
        case ENEMY3_THROW: {
            //printf("THROW\n");
            int frame = gif_status[ENEMY3_THROW]->display_index;
            int last = gif_status[ENEMY3_THROW]->frames_count - 1;

            if (frame == 2 && !active_proj) {
                Elements* pro = new Projectile3(
                    Projectile3_2_L,
                    dir ? (x + width - 100) : (x + 20),
                    y + height / 2 + 20,
                    dir ? 5 : -5
                );

                Projectile3* p = dynamic_cast<Projectile3*>(pro);
                if (p) p->is_enemy_projectile = true;
                scene->addElement(p);

                al_play_sample_instance(throw_sound);
                active_proj = true;
            }

            if (frame == last) {
                gif_status[ENEMY3_THROW]->display_index = 0;
                active_proj = false;

                if (abs(dx) <= ENEMY3_MELEE_RANGE)
                    state = ENEMY3_IDLE;
                else
                    state = ENEMY3_MOVE;
            }
            break;
        }

        // ----------------- DEAD -----------------
        case ENEMY3_DEAD:
            if (death_time == 0) death_time = al_get_time();
            else if (gif_status[ENEMY3_DEAD]->done &&
                al_get_time() - death_time >= 1.0)
                dele = true;
            break;

        default:
            break;
    }
}


void Enemy3::Draw(){
    if (hp <= 0) return;

    ALLEGRO_BITMAP* frame = algif_get_bitmap(gif_status[state], al_get_time());
    if (!frame) return;

    int flags = dir ? 0 : ALLEGRO_FLIP_HORIZONTAL;
    int draw_y = y;
 if (got_hit && al_get_time() - hit_time < 0.2)
        al_draw_tinted_bitmap(frame, al_map_rgba(255,0,0,200), x, draw_y, flags);
    else
        al_draw_bitmap(frame, x, draw_y, flags);

    // HP bar
    int bar_x = x + 80;
    int bar_y = y + 80;
    float ratio = (float)hp / maxhp;

    al_draw_filled_rectangle(bar_x, bar_y, bar_x + 100, bar_y+10, al_map_rgb(255,255,255));
    al_draw_filled_rectangle(bar_x, bar_y, bar_x + (100*ratio), bar_y+10, al_map_rgb(150,0,0));
    /*
    if (hitbox) {
        if (hitbox->getType() == ShapeType::RECTANGLE) {
            Rectangle* r = static_cast<Rectangle*>(hitbox);
            al_draw_rectangle(
                r->x1, r->y1,
                r->x2, r->y2,
                al_map_rgb(0, 255, 0), 2
            );
        }
    }*/
}

void Enemy3::Interact(){
}














