#include "Character2.h"
#include "projectile.h"
#include "projectile2.h"
#include "../shapes/Rectangle.h"
#include "../scene/gamescene.h"
#include "enemy.h"
#include "../scene/sceneManager.h"
#include "../algif5/algif.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "../element/elements_factory.h"

#include <iostream>
#define cool_l 600
#define cool_k 480
#define cool_j 300
// 建立角色的 wrapper 函式

Character2::Character2()
    :Elements(Character2_L), x(30), y(0), width(0), height(0),
      blood(100), level(0), levelup_points(0), add_blood(10),
      dir(false), is_jumping(false), jump_start_time(0.0), ground_y(0),
      cool_J(0), cool_K(0), cool_L(0),
      atk_type(0), new_proj(false), state(STOP)
{
    // Load GIF animations using algif_load_animation
    const char* states[3] = { "stop", "move", "atk" };
    for (int i = 0; i < 3; ++i) {
        std::string path = "assets/image/chara2_" + std::string(states[i]) + ".gif";
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
    hitbox = new Rectangle(x, y, x + width, y + height);

    // Load attack sound
    ALLEGRO_SAMPLE* sample = al_load_sample("assets/sound/atk_sound.wav");
    atk_Sound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(atk_Sound, al_get_default_mixer());
}

Character2::~Character2()
{
    if (atk_Sound) al_destroy_sample_instance(atk_Sound);
    for (int i = 0; i < 3; ++i)
        algif_destroy_animation(gif_status[i]);
    delete hitbox;
}

void Character2::Update()
{
    if (blood <= 0) return; // dead

    // Level up
    if (levelup_points >= (level + 1) * 10) {
        levelup_points -= (level + 1) * 10;
        level++;
        blood += add_blood;
    }
    if (cool_J > 0) --cool_J;
    if (cool_K > 0) --cool_K;
    if (cool_L > 0) --cool_L;

    // Jumping
    if (is_jumping) {
        double elapsed = al_get_time() - jump_start_time;
        int dx = 0;

        if (key_state[ALLEGRO_KEY_UP] && elapsed < 0.5)
            update_position(0, -5);
        else if (y < ground_y)
            update_position(0, 5);

        if (key_state[ALLEGRO_KEY_LEFT]) { dx -= 5; dir = false; }
        if (key_state[ALLEGRO_KEY_RIGHT]) { dx += 5; dir = true; }
        update_position(dx, 0);

        if (y >= ground_y) {
            y = ground_y;
            is_jumping = false;
            update_position(0, 0);
        }

        int new_atk_type = 0;
        if (key_state[ALLEGRO_KEY_J] && cool_j == 0) { new_atk_type = 1; cool_J = cool_j; }
        else if (key_state[ALLEGRO_KEY_K]) new_atk_type = 2;
        else if (key_state[ALLEGRO_KEY_L]) new_atk_type = 3;

        if (!new_proj && new_atk_type != 0) {
            atk_type = new_atk_type;
            state = ATK;
            trigger_attack(atk_type);
            new_proj = true;
        }
        return;
    }

    // State machine
    switch (state) {
        case STOP:
            if (key_state[ALLEGRO_KEY_J] && cool_J == 0) { state = ATK; atk_type = 1; new_proj = false; cool_J = cool_j; }
            else if (key_state[ALLEGRO_KEY_K] && cool_K == 0) { state = ATK; atk_type = 2; new_proj = false; cool_K = cool_k; }
            else if (key_state[ALLEGRO_KEY_L] && cool_L == 0) { state = ATK; atk_type = 3; new_proj = false; cool_L = cool_l; }
            else if (key_state[ALLEGRO_KEY_LEFT]) { dir = false; state = MOVE; }
            else if (key_state[ALLEGRO_KEY_RIGHT] || key_state[ALLEGRO_KEY_UP] || key_state[ALLEGRO_KEY_DOWN]) { dir = true; state = MOVE; }
            break;

        case MOVE: {
            int dx = 0;
            if (key_state[ALLEGRO_KEY_LEFT]) { dx -= 5; dir = false; }
            if (key_state[ALLEGRO_KEY_RIGHT]) { dx += 5; dir = true; }
            update_position(dx, 0);

            if (key_state[ALLEGRO_KEY_UP] && !is_jumping && y == ground_y) {
                is_jumping = true;
                jump_start_time = al_get_time();
                return;
            }

            if (key_state[ALLEGRO_KEY_J] && cool_J == 0) { state = ATK; atk_type = 1; new_proj = false; cool_J = cool_j; }
            else if (key_state[ALLEGRO_KEY_K] && cool_K == 0) { state = ATK; atk_type = 2; new_proj = false; cool_K = cool_k; }
            else if (key_state[ALLEGRO_KEY_L] && cool_L == 0) { state = ATK; atk_type = 3; new_proj = false; cool_L = cool_l; }

            if (gif_status[state]->done) state = STOP;
        } break;

        case ATK: {
            ALGIF_ANIMATION* a = gif_status[ATK];
            int total = a->frames_count;

            if (a->display_index == 2 && !new_proj) {
                trigger_attack(atk_type);
                new_proj = true;
            }

            if (a->display_index >= total - 1) {
                state = STOP;
                new_proj = false;
                atk_type = 0;
                a->display_index = 0;
                break;
            }
            break;
        }
    }
}

void Character2::Draw()
{
    ALLEGRO_BITMAP* frame = algif_get_bitmap(gif_status[state], al_get_time());
    if (frame) {
        al_draw_bitmap(frame, x, y, dir ? ALLEGRO_FLIP_HORIZONTAL : 0);
    }
}

void Character2::Interact(){
}



void Character2::trigger_attack(int atk)
{
    al_play_sample_instance(atk_Sound);
    Elements* pro = nullptr;

    switch (atk) {
        case 1:
            if (dir) pro = new Projectile(Projectile_J, x + width / 2, y + 30, 2, this);
            else pro = new Projectile(Projectile_J, x - 150, y + 30, -2, this);
            scene->addElement(pro);
            break;
        case 2: 
            if (dir) {
                pro = new Projectile(Projectile_K, x + width / 2, y + 70, 5, this);
                scene->addElement(pro);
                printf("ADDED PRO\n");
                Elements* tail1 = new Projectile(Projectile_K, x + width / 2 + 10, y + 15, 3, this);
                Elements* tail2 = new Projectile(Projectile_K, x + width / 2 + 10, y + 125, 3, this);
                scene->addElement(tail1);
                printf("ADDED tail1\n");
                scene->addElement(tail2);
                printf("ADDED PRO\n");
            } else {
                pro = new Projectile(Projectile_K, x - 10, y + 80, -5, this);
                scene->addElement(pro);
                Elements* tail1 = new Projectile(Projectile_K, x - 30, y + 15, -3, this);
                Elements* tail2 = new Projectile(Projectile_K, x - 30, y + 125, -3, this);
                scene->addElement(tail1);
                scene->addElement(tail2);
            }
            break;
        case 3: // V attack
            if (dir) pro = new Projectile(Projectile_L, x + width, y + 10, 5, this);
            else pro = new Projectile(Projectile_L, x - 180, y + 10, -5, this);
            scene->addElement(pro);
            break;
    }
}


void Character2::update_position(int dx, int dy)
{
    x += dx;
    y += dy;
    hitbox->update_position(dx, dy);
}
