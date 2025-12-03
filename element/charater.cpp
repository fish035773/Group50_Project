#include "charater.h"
#include "projectile.h"
#include "../shapes/Rectangle.h"
//character 圖片敲可愛的 很喜翻~
#include "enemy.h"
#include "../scene/sceneManager.h"
#include "../algif5/algif.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "../element/elements_factory.h"

#include <iostream>
#define cool_v 600
#define cool_c 480
#define cool_x 300
// 建立角色的 wrapper 函式

Character::Character()
    : x(30), y(0), width(0), height(0),
      blood(100), level(0), levelup_points(0), add_blood(10),
      dir(false), is_jumping(false), jump_start_time(0.0), ground_y(0),
      cool_X(0), cool_C(0), cool_V(0),
      atk_type(0), new_proj(false), state(STOP)
{
    // Load GIF animations using algif_load_animation
    const char* states[3] = { "stop", "move", "atk1" };
    for (int i = 0; i < 3; ++i) {
        std::string path = "assets/image/chara_" + std::string(states[i]) + ".gif";
        gif_status[i] = algif_load_animation(path.c_str());
        if (!gif_status[i]) {
            std::cerr << "Failed to load GIF: " << path << std::endl;
        }
    }

    width = gif_status[0]->width;
    height = gif_status[0]->height;
    y = HEIGHT - height - 20;
    ground_y = y;

    // Create hitbox
    hitbox = New_Rectangle(x, y, x + width, y + height);

    // Load attack sound
    ALLEGRO_SAMPLE* sample = al_load_sample("assets/sound/atk_sound.wav");
    atk_Sound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(atk_Sound, al_get_default_mixer());
}


Character::~Character()
{
    if (atk_Sound) al_destroy_sample_instance(atk_Sound);
    for (int i = 0; i < 3; ++i)
        algif_destroy_animation(gif_status[i]);
    delete hitbox;
}

void Character::update()
{
    if (blood <= 0) return; // dead

    // Level up
    if (levelup_points >= (level + 1) * 10) {
        levelup_points -= (level + 1) * 10;
        level++;
        blood += add_blood;
    }

    // Cooldowns
    if (cool_C > 0) --cool_C;
    if (cool_V > 0) --cool_V;
    if (cool_X > 0) --cool_X;

    // Enemy AI call (if needed)
    enemy_charater(x);

    // Jumping
    if (is_jumping) {
        double elapsed = al_get_time() - jump_start_time;
        int dx = 0;

        if (key_state[ALLEGRO_KEY_W] && elapsed < 0.5)
            update_position(0, -5);
        else if (y < ground_y)
            update_position(0, 5);

        if (key_state[ALLEGRO_KEY_A]) { dx -= 5; dir = false; }
        if (key_state[ALLEGRO_KEY_D]) { dx += 5; dir = true; }
        update_position(dx, 0);

        if (y >= ground_y) {
            y = ground_y;
            is_jumping = false;
            update_position(0, 0);
        }

        int new_atk_type = 0;
        if (key_state[ALLEGRO_KEY_X] && cool_X == 0) {
            new_atk_type = 1;
            cool_X = cool_x;
        }
        else if (key_state[ALLEGRO_KEY_C] && cool_C == 0) {
            new_atk_type = 2;
            cool_C = cool_c;
        }
        else if (key_state[ALLEGRO_KEY_V] && cool_V == 0) {
            new_atk_type = 3;
            cool_V = cool_v;
        }

        if (!new_proj && new_atk_type != 0) {
            atk_type = new_atk_type;
            state = ATK1;
            trigger_attack(atk_type);
            new_proj = true;
        }
        return;
    }

    // State machine
    switch (state) {
        case STOP:
            if (key_state[ALLEGRO_KEY_X] && cool_X == 0) { state = ATK1; atk_type = 1; new_proj = false; cool_X = cool_x; }
            else if (key_state[ALLEGRO_KEY_C] && cool_C == 0) { state = ATK1; atk_type = 2; new_proj = false; cool_C = cool_c; }
            else if (key_state[ALLEGRO_KEY_V] && cool_V == 0) { state = ATK1; atk_type = 3; new_proj = false; cool_V = cool_v; }
            else if (key_state[ALLEGRO_KEY_A]) { dir = false; state = MOVE; }
            else if (key_state[ALLEGRO_KEY_D] || key_state[ALLEGRO_KEY_W] ) { dir = true; state = MOVE; }
            break;

        case MOVE: {
            int dx = 0;
            if (key_state[ALLEGRO_KEY_A]) { dx -= 5; dir = false; }
            if (key_state[ALLEGRO_KEY_D]) { dx += 5; dir = true; }
            update_position(dx, 0);

            if (key_state[ALLEGRO_KEY_W] && !is_jumping && y == ground_y) {
                is_jumping = true;
                jump_start_time = al_get_time();
                return;
            }

            if (key_state[ALLEGRO_KEY_X] && cool_X == 0) { state = ATK1; atk_type = 1; new_proj = false; cool_X = cool_x; }
            else if (key_state[ALLEGRO_KEY_C] && cool_C == 0) { state = ATK1; atk_type = 2; new_proj = false; cool_C = cool_c; }
            else if (key_state[ALLEGRO_KEY_V] && cool_V == 0) { state = ATK1; atk_type = 3; new_proj = false; cool_V = cool_v; }

            if (gif_status[state]->done) state = STOP;
        } break;

        case ATK1:
            if (gif_status[ATK1]->display_index == 2 && !new_proj) {
                trigger_attack(atk_type);
                new_proj = true;
            }
            
            if (gif_status[ATK1]->display_index == 4) {
                state = is_jumping ? MOVE : STOP;
                new_proj = false;
                atk_type = 0;
                break;

            }
            
            break;
    }
}

void Character::draw()
{
    ALLEGRO_BITMAP* frame = algif_get_bitmap(gif_status[state], al_get_time());
    if (frame) {
        al_draw_bitmap(frame, x, y, dir ? ALLEGRO_FLIP_HORIZONTAL : 0);
    }
}

void Character::interact()
{
    // Placeholder
}

void Character::trigger_attack(int atk)
{
    al_play_sample_instance(atk_Sound);
    Elements* pro = nullptr;

    switch (atk) {
        case 1: // X attack
            if (dir) pro = New_Projectile(Projectile_X, x + width, y + 30, 2, this);
            else pro = New_Projectile(Projectile_X, x - 150, y + 30, -2, this);
            _Register_elements(scene, pro);

            // --- debug print ---
            if (pro && pro->pDerivedObj) {
                Projectile* p = (Projectile*)pro->pDerivedObj;
                printf("[DEBUG] X attack projectile created at (%d,%d), img=%p\n", p->x, p->y, p->img);
            } else {
                printf("[DEBUG] X attack projectile creation FAILED\n");
            }
            break;

        case 2: // C attack
            if (dir) {
                pro = New_Projectile(Projectile_C, x + width + 10, y + 70, 5, this);
                _Register_elements(scene, pro);
                Elements* tail1 = New_Projectile(Projectile_C, x + width + 30, y + 15, 3, this);
                Elements* tail2 = New_Projectile(Projectile_C, x + width + 30, y + 125, 3, this);
                _Register_elements(scene, tail1);
                _Register_elements(scene, tail2);

                printf("[DEBUG] C attack projectiles created: main=%p, tail1=%p, tail2=%p\n",
                       pro, tail1, tail2);
            } else {
                pro = New_Projectile(Projectile_C, x - 120, y + 80, -5, this);
                _Register_elements(scene, pro);
                Elements* tail1 = New_Projectile(Projectile_C, x - 100, y + 15, -3, this);
                Elements* tail2 = New_Projectile(Projectile_C, x - 100, y + 125, -3, this);
                _Register_elements(scene, tail1);
                _Register_elements(scene, tail2);

                printf("[DEBUG] C attack projectiles created: main=%p, tail1=%p, tail2=%p\n",
                       pro, tail1, tail2);
            }
            break;

        case 3: // V attack
            if (dir) pro = New_Projectile(Projectile_V, x + width, y + 10, 5, this);
            else pro = New_Projectile(Projectile_V, x - 180, y + 10, -5, this);
            _Register_elements(scene, pro);

            
            break;
    }
}


void Character::update_position(int dx, int dy)
{
    x += dx;
    y += dy;

    if (hitbox) {
        hitbox->update_center_x(hitbox, dx); // 傳入 self + dx
        hitbox->update_center_y(hitbox, dy); // 傳入 self + dy
    }
}