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
#include "charater.h"
#include "Character2.h"
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

    hitbox->update_position(dx, dy);
}

int player_center_x1;
void enemy_charater(int dx){
    player_center_x1 = dx;
}

void Enemy::Update() {
    if (hp <= 0) return;

    int speed = 2;
    int enemy_center_x = x + width / 2;
    int dx = player_center_x1 - enemy_center_x;

    double current_time = al_get_time();
    if (!can_attack && current_time - spawn_time >= 1.0)
        can_attack = true;

    if (can_attack && abs(dx) <= ENEMY_ATTACK_RANGE) {
        dir = dx >= 0;

        if (state != ENEMY_ATK) {
            state = ENEMY_ATK;
            active_proj = false;
            al_play_sample_instance(atk_sound);
        }
    }

    switch (state) {

        case ENEMY_IDLE:
            printf("IDLE\n");
            if (abs(dx) <= ENEMY_ATTACK_RANGE && can_attack) {
                dir = dx >= 0;
                state = ENEMY_ATK;
                active_proj = false;
                al_play_sample_instance(atk_sound);
            } else {
                state = ENEMY_MOVE;
            }
            break;

        case ENEMY_MOVE:
            //printf("MOVE%d\n", can_attack);
            if (abs(dx) <= ENEMY_ATTACK_RANGE && can_attack) {
                dir = dx >= 0;
                state = ENEMY_ATK;
                active_proj = false;
                al_play_sample_instance(atk_sound);
            } else {
                if (chasing) {
                    dir = (dx >= 0);
                    int move_speed = (dx > 0) ? speed : -speed;
                    update_position(move_speed, 0);
                } else {
                    if (!dir) {
                        update_position(-speed, 0);
                        if (x < 400) dir = true;
                    } else {
                        update_position(speed, 0);
                        if (x > 700) dir = false;
                    }
                }
            }
            break;

        case ENEMY_ATK: {
            
            int frame = gif_status[ENEMY_ATK]->display_index;

            if (frame == 2 && !active_proj) {
                //printf("ATK\n");
                int projectile_x = dir ?
                    x + width - 100 :
                    x + 20;

                Elements* pro = new Projectile2(
                    Projectile2_L,
                    projectile_x,
                    y + height/2 - 20,
                    dir ? 5 : -5
                );

                Projectile2* p = dynamic_cast<Projectile2*>(pro);
                if (p) p->is_enemy_projectile = true;
                scene->addElement(p);

                active_proj = true;
            }

            if (gif_status[ENEMY_ATK]->done) {
                gif_status[ENEMY_ATK]->done = false;
                gif_status[ENEMY_ATK]->display_index = 0;
                active_proj = false;
                //printf("DONE\n");
                if (abs(dx) <= ENEMY_ATTACK_RANGE)
                    state = ENEMY_IDLE;
                else
                    state = ENEMY_MOVE;
            }
            break;
        }

        case ENEMY_DEAD:
            if (death_time == 0) death_time = al_get_time();
            else if (gif_status[ENEMY_DEAD]->done &&
                    al_get_time() - death_time >= 1.0)
                dele = true;
            break;
    }
}

void Enemy::Draw() {
    if (hp <= 0) return;

    ALLEGRO_BITMAP* frame = algif_get_bitmap(gif_status[state], al_get_time());
    if (!frame) return;

    // 基本繪製
    al_draw_bitmap(frame, x, y, (dir ? 0 : ALLEGRO_FLIP_HORIZONTAL));

    if (state == ENEMY_ATK && gif_status[state]->display_index == 2)
        al_play_sample_instance(atk_sound);

    // HP bar
    int bar_width = 100;
    int bar_height = 10;
    int bar_x = x + 80;
    int bar_y = y;

    float hp_ratio = (float)hp / maxhp;

    al_draw_filled_rectangle(
        bar_x, bar_y,
        bar_x + bar_width,
        bar_y + bar_height,
        al_map_rgb(225, 225, 225)
    );

    al_draw_filled_rectangle(
        bar_x, bar_y,
        bar_x + (int)(bar_width * hp_ratio),
        bar_y + bar_height,
        al_map_rgb(150, 0, 0)
    );

    if (got_hit && al_get_time() - hit_time < 0.2) {
        al_draw_tinted_bitmap(
            frame,
            al_map_rgba(255, 0, 0, 200),
            x, y,
            (dir ? 0 : ALLEGRO_FLIP_HORIZONTAL)
        );
    }
}


void Enemy::Interact()
{
    if (!alive || dying) return;
    if (!scene) return;

    for (Elements* obj : scene->getAllElements()) {

        if (obj->dele) continue;

        // ---- find projectile (Projectile2) ----
        Projectile2* proj = dynamic_cast<Projectile2*>(obj);
        if (!proj) continue;

        // ignore if this projectile belongs to enemies
        if (proj->is_enemy_projectile) 
            continue;

        // hitbox overlap check
        if (hitbox->overlap(*proj->hitbox)) {
            //printf("HURT\n");
            // projectile disappears
            obj->dele = true;

            // damage
            hp--;
            got_hit = true;
            hit_time = al_get_time();

            // death state transition
            if (hp <= 0 && !dying) {
                alive = false;
                dying = true;
                state = ENEMY_DEAD;
                printf("DIED\n");
                gif_status[ENEMY_DEAD]->display_index = 0;
                gif_status[ENEMY_DEAD]->done = false;
                death_time = al_get_time();
                return;
            }
        }
    }
}


Elements* New_Enemy(int label)
{
    return new Enemy(label);
}