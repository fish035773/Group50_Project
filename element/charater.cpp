#include "charater.h"
#include "../scene/gamescene.h"
#include "projectile.h"
#include "projectile2.h"
#include "../shapes/Rectangle.h"
#include "element.h"
#include "enemy.h"
#include "../scene/sceneManager.h"
#include "../algif5/algif.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "../element/elements_factory.h"
#include <iostream>
#define cool_v 10 * 80
#define cool_c 8 * 80
#define cool_x 5 * 80
#define hit_coins_add 10
#define kill_coins_add 50
#define text_size 24
#define text_space 30
#define text_x 910
// 建立角色的 wrapper 函式

Character::Character()
    : Elements(Character_L), x(30), y(0), width(0), height(0),
      blood(100), level(0), levelup_points(0), add_blood(10),
      dir(false), is_jumping(false), jump_start_time(0.0), ground_y(0),
      cool_X(0), cool_C(0), cool_V(0),
      atk_type(0), new_proj(false), state(STOP), died(false)
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
    y = HEIGHT - height - 30;
    ground_y = y;
    
    // Create hitbox
    hitbox = new Rectangle(x, y, x + width, y + height);

    // Load attack sound
    ALLEGRO_SAMPLE* sample = al_load_sample("assets/sound/atk_sound.wav");
    atk_Sound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(atk_Sound, al_get_default_mixer());

    // ==== LOAD FONTS ====
    al_init_font_addon();
    al_init_ttf_addon();
    font = al_load_ttf_font("assets/font/Gil.ttf", text_size, 0);
    coins = 0;

    hp_bg = al_load_bitmap("assets/image/chara_hp_bg.png");
    hp_bar = al_load_bitmap("assets/image/chara_hp.png");
    exp_bar = al_load_bitmap("assets/image/exp_bar.png");

    if (!hp_bg) std::cerr << "Failed to load HP BG!\n";
    if (!hp_bar) std::cerr << "Failed to load HP BAR!\n";

}

Character::~Character()
{
    if (atk_Sound) al_destroy_sample_instance(atk_Sound);
    for (int i = 0; i < 3; ++i)
        algif_destroy_animation(gif_status[i]);
    delete hitbox;

    if (hp_bg) al_destroy_bitmap(hp_bg);
    if (hp_bar) al_destroy_bitmap(hp_bar);
}

void Character::Update()
{
    if (blood <= 0) {
        died = true;
        return;
    }
    
    // Level up
    if (levelup_points >= (level + 1) * 10) {
        levelup_points -= (level + 1) * 10;
        level++;
        blood += add_blood;
        coins += 100;
    }

    // Cooldowns
    if (cool_C > 0) --cool_C;
    if (cool_V > 0) --cool_V;
    if (cool_X > 0) --cool_X;

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
    //printf("Current State: %d\n", state);
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
            //printf("FDSA\n");
            if (gif_status[ATK1]->display_index == 4) {
                gif_status[ATK1]->done = true;
            }
            
            if (gif_status[ATK1]->done) {
                state = is_jumping ? MOVE : STOP;
                new_proj = false;
                atk_type = 0;
                gif_status[ATK1]->done = false;
                gif_status[ATK1]->display_index = 0;
                break;
            }

            /*
            printf("[DEBUG] ATK1 state: display_index=%d, done=%d, new_proj=%d\n",
                   gif_status[ATK1]->display_index, gif_status[ATK1]->done, new_proj);
            */
            break;
        default:
            break;
    }
}

void Character::Draw()
{
    ALLEGRO_BITMAP* frame = algif_get_bitmap(gif_status[state], al_get_time());
    if (frame) {
        al_draw_bitmap(frame, x, y, dir ? ALLEGRO_FLIP_HORIZONTAL : 0);
    }
    int text_y = 10;
    if(blood > 0){
        char buf[64];
        sprintf(buf, "Character 1");
        al_draw_text(font, al_map_rgb(84, 74, 67), text_x, text_y, 0, buf);
        text_y += text_space;
        sprintf(buf, "coins: %d", coins);
        al_draw_text(font, al_map_rgb(84, 74, 67), text_x, text_y, 0, buf);
    }

    // === DRAW HP BAR ===
    float hp_percent = (float)blood / 100.0f;
    if (hp_percent < 0) hp_percent = 0;
    if (hp_percent > 1) hp_percent = 1;

    int bar_width = 461;
    int bar_height = 31;

    int ui_hp_x = 300;
    int ui_hp_y = 10;

    al_draw_bitmap(hp_bg, ui_hp_x, ui_hp_y, 0);
    int current_width = (int)(bar_width * hp_percent);

    if (current_width > 0) {
        al_draw_bitmap_region(
            hp_bar,
            0, 0,
            current_width,
            bar_height,
            ui_hp_x, ui_hp_y,
            0
        );
    }
}

void Character::Interact(){}

void Character::trigger_attack(int atk)
{
    al_play_sample_instance(atk_Sound);

    Projectile* pro = nullptr;

    switch (atk) {
        // ================================
        // 1. X attack
        // ================================
        case 1:
            if (dir)
                pro = new Projectile(Projectile::Projectile_X, x + width, y + 30, 2, this);
            else
                pro = new Projectile(Projectile::Projectile_X, x - 150, y + 30, -2, this);

            scene->addElement(pro);
            printf("[DEBUG] X projectile created at (%d,%d)\n", pro->x, pro->y);
            break;

        // ================================
        // 2. C attack (帶三發)
        // ================================
        case 2:
            if (dir) {
                Projectile* main = new Projectile(Projectile::Projectile_C, x + width + 10, y + 70, 5, this);
                Projectile* t1   = new Projectile(Projectile::Projectile_C, x + width + 30, y + 15, 3, this);
                Projectile* t2   = new Projectile(Projectile::Projectile_C, x + width + 30, y + 125, 3, this);

                scene->addElement(main);
                scene->addElement(t1);
                scene->addElement(t2);

                printf("[DEBUG] C attack 3 projectiles created.\n");
            }
            else {
                Projectile* main = new Projectile(Projectile::Projectile_C, x - 120, y + 80, -5, this);
                Projectile* t1 = new Projectile(Projectile::Projectile_C, x - 100, y + 15, -3, this);
                Projectile* t2 = new Projectile(Projectile::Projectile_C, x - 100, y + 125, -3, this);

                scene->addElement(main);
                scene->addElement(t1);
                scene->addElement(t2);

                printf("[DEBUG] C attack 3 projectiles created (left side).\n");
            }
            break;

        // ================================
        // 3. V attack
        // ================================
        case 3:
            if (dir)
                pro = new Projectile(Projectile::Projectile_V, x + width, y + 10, 5, this);
            else
                pro = new Projectile(Projectile::Projectile_V, x - 180, y + 10, -5, this);

            scene->addElement(pro);
            printf("[DEBUG] V projectile created at (%d,%d)\n", pro->x, pro->y);
            break;
    }
}


void Character::update_position(int dx, int dy)
{
    x += dx;
    y += dy;

    if (hitbox) {
        hitbox->update_position(dx, dy);
    }
}

void Character::OnHitEnemy(int damage, bool kill)
{
    levelup_points++;
    coins += (kill ? kill_coins_add : hit_coins_add);
}
