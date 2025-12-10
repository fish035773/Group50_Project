#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "boss.h"
#include "projectile2.h"
#include <math.h>
#include "../scene/sceneManager.h"
#include "../shapes/Rectangle.h"
#include "../algif5/algif.h"
#include "../scene/gamescene.h"
#include "charater.h"
#include "Character2.h"
#include "../element/projectile4.h"
#include <queue>
#define maxhp 50

struct ScheduledDrop {
    double time;
};

std::queue<ScheduledDrop> drop_queue;

Boss::Boss(int label, int speed): Elements(label), speed(speed){
    char state_string[4][10] = {"stop", "move", "attack", "dead"};
    for (int i = 0; i < 4; i++){
        char buffer[100];
        sprintf(buffer, "assets/image/boss.gif");
        gif_status[i] = algif_load_animation(buffer);
        if(!gif_status[i]) printf("[boss.cpp] Failed to load boss.gif\n");
    }

    ALLEGRO_SAMPLE *sample = al_load_sample("assets/sound/fire_atk_sound.mp3");
    atk_sound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(atk_sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(atk_sound, al_get_default_mixer());
    
    scale = 2.5;

    width = gif_status[0]->width * scale;
    height = gif_status[0]->height * scale;

    x = (WIDTH - width) / 2;
    y = HEIGHT - height - 30;

    hitbox = new Rectangle(
        x * 1.0 + 50, y * 1.0,
        x + width * 0.8 + 50,
        y + height * 1.0
    );

    dir = true;
    state = BOSS_IDLE;
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

    last_attack_time = al_get_time();
    attack_interval = 2.0;
}

Boss::~Boss(){
    al_destroy_sample_instance(atk_sound);
    for (int i = 0; i < 4; i++)
        algif_destroy_animation(gif_status[i]);

    delete hitbox;
}

void Boss::update_position(int dx, int dy){
    x += dx;
    y += dy;

    hitbox->update_position(dx, dy);
}

void Boss::Update() {
    if (hp <= 0) return;

    double now = al_get_time();

    if (now - last_attack_time > attack_interval) {
        SpawnFallingAttack();
        last_attack_time = now;
    }

    double now2 = now;

    while (!drop_queue.empty() && drop_queue.front().time <= now2) {
        int range_left = 0;
        int range_right = WIDTH - 200;
        int drop_x = range_left + rand() % (range_right - range_left);

        Projectile4* p = new Projectile4(
            Projectile4_L,
            drop_x,
            -100,
            0
        );

        p->v = 10;
        p->falling = true;

        scene->addElement(p);

        drop_queue.pop();
    }
}

void Boss::SpawnFallingAttack() {
double now = al_get_time();

    int count = 2 + rand() % 2;  // 2 or 3

    for (int i = 0; i < count; i++) {
        ScheduledDrop d;
        d.time = now + i * 0.3;

        drop_queue.push(d);
    }
}

void Boss::Draw() {
    if (hp <= 0) return;

    ALLEGRO_BITMAP* frame = algif_get_bitmap(gif_status[state], al_get_time());
    if (!frame) return;

    al_draw_scaled_bitmap(
        frame,
        0, 0,
        al_get_bitmap_width(frame),
        al_get_bitmap_height(frame),
        x,
        y,
        al_get_bitmap_width(frame) * scale,
        al_get_bitmap_height(frame) * scale,
        (dir ? 0 : ALLEGRO_FLIP_HORIZONTAL)
    );

    if (state == BOSS_ATK && gif_status[state]->display_index == 2)
        al_play_sample_instance(atk_sound);

    // HP bar
    int bar_width = 400;
    int bar_height = 30;
    int bar_x = (WIDTH - bar_width) / 2;
    int bar_y = HEIGHT - 200;

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
    
    if (hitbox) {
        if (hitbox->getType() == ShapeType::RECTANGLE) {
            Rectangle* r = static_cast<Rectangle*>(hitbox);
            al_draw_rectangle(
                r->x1, r->y1,
                r->x2, r->y2,
                al_map_rgb(0, 255, 0), 2
            );
        }
    }
}

void Boss::Interact()
{}