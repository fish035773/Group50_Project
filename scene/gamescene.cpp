
#include <allegro5/allegro_audio.h>
#include "gamescene.h"
#include "scene.h"
#include "../global.h" 
#include "../element/element.h"
#include "../element/charater.h"
#include "../element/floor.h"
#include "../element/projectile.h"
#include "../element/Character2.h"
#include "../element/elements_factory.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "../element/projectile2.h"
#include "sceneManager.h"
#include "../element/enemy.h"
#include "../element/enemy2.h"
#include "../element/enemy3.h"
#include <algorithm>
#define FPS 60
#define barlong 510
Scene *current_game_scene = NULL; //pointer to reference active scene
int i = 0;
int enemy_count = 0;

GameScene::GameScene(int label)
    : Scene(label)
{
    // ==== INITIAL STATE ====
    round_counter = 1;
    round_triggered = true;
    round_timer = 0;
    enemy_defeated = false;
    round_locked = false;
    enemies_spawned = false;

    printf("[GameScene] Constructor: round state reset.\n");

    // ==== LOAD BACKGROUND ====
    background[0] = al_load_bitmap("assets/image/round1_scene.png");
    background[1] = al_load_bitmap("assets/image/round2_scene.png");
    background[2] = al_load_bitmap("assets/image/round3_scene.png");
    // ==== LOAD ROUND IMAGES ====
    round_images[0] = al_load_bitmap("assets/image/round_1.png");
    round_images[1] = al_load_bitmap("assets/image/round_2.png");
    round_images[2] = al_load_bitmap("assets/image/round_3.png");

    // ==== LOAD FONTS ====
    al_init_font_addon();
    al_init_ttf_addon();
    font = al_load_ttf_font("assets/font/Consolas.ttf", 24, 0);

    // ==== LOAD SOUNDS ====
    round_sounds[0] = al_load_sample("assets/sound/round_1.mp3");
    round_sounds[1] = al_load_sample("assets/sound/round_2.mp3");
    round_sounds[2] = al_load_sample("assets/sound/round_3.mp3");

    round_bgm[0] = al_load_sample("assets/sound/bgm_round1.mp3");
    round_bgm[1] = al_load_sample("assets/sound/bgm_round2.mp3");
    round_bgm[2] = al_load_sample("assets/sound/bgm_round3.mp3");

    // ==== START FIRST BGM ====
    if (round_bgm[0]) {
        round_bgm_instance = al_create_sample_instance(round_bgm[0]);
        al_set_sample_instance_playmode(round_bgm_instance, ALLEGRO_PLAYMODE_LOOP);
        al_attach_sample_instance_to_mixer(round_bgm_instance, al_get_default_mixer());
        al_play_sample_instance(round_bgm_instance);
    }

    // ==== REGISTER ELEMENTS ====
    addElement(new Floor(Floor_L));
    addElement(new Character());
    addElement(new Character2());
    printf("Added elements: \n");
    for (auto* e : elements){
        printf(" - label = %d\n", e->label);
    }
    printf("[GameScene] Constructor finished.\n");
}

void GameScene::Update() {
    Scene::Update();
    // === GLOBAL INTERACTION ===
    for (auto* ele : elements){
        ele->Interact();
        if(ele->label == Character2_L){
            Character2* c = dynamic_cast<Character2*>(ele);

            if(c && c->died){
                if (round_bgm_instance){
                    al_stop_sample_instance(round_bgm_instance);
                    al_destroy_sample_instance(round_bgm_instance);
                    printf("Round BGM stopped and destroyed.\n");
                }
                create_scene(DeathScene_L);
            }
        }
        if(ele->label == Character_L){
            Character *c = dynamic_cast<Character*>(ele);
            
            if(c && c->died){
                if (round_bgm_instance){
                    al_stop_sample_instance(round_bgm_instance);
                    al_destroy_sample_instance(round_bgm_instance);
                    printf("Round BGM stopped and destroyed.\n");
                }
                create_scene(DeathScene_L);
            }
        }
    }
    elements.erase(
        std::remove_if(elements.begin(), elements.end(),
            [&](Elements* ele){
                if (ele->dele) {
                    delete ele;
                    return true;
                }
                return false;
            }),
        elements.end()
    );

    if (round_triggered) {
        round_timer++;

        if (round_timer > FPS * 2) {
            round_triggered = false;
            round_timer = 0;
            printf("[GameScene] Round %d banner done.\n", round_counter);
        }

        return;   // 圖片顯示時不做其他邏輯
    }
    
    // =====================================================
    // 2. SPAWN ENEMIES WHEN READY
    // =====================================================
    if (!enemies_spawned && !round_locked) {
        printf("[GameScene] Spawning enemies for round %d...\n", round_counter);
        int spacing = 0;

        switch (round_counter) {
        case 1:
            enemy_count = 3;
            spacing = 100;
            for (int i = 0; i < enemy_count; i++) {
                Enemy* e = new Enemy(Enemy_L, i);
                e->update_position(i * spacing, 0);
                addElement(e);
            }
            break;

        case 2:
            enemy_count = 2;
            spacing = 150;
            for (int i = 0; i < enemy_count; i++) {
                Enemy2* e = new Enemy2(Enemy2_L, i + 3);
                e->update_position(i * spacing, 0);
                addElement(e);
            }
            break;

        case 3:
            enemy_count = 1;
            for (int i = 0; i < enemy_count; i++) {
                Enemy3* e = new Enemy3(Enemy3_L, 5);
                e->update_position(i * spacing, 0);
                addElement(e);
            }
            break;
        }

        enemies_spawned = true;
        enemy_defeated = false;
        printf("[GameScene] Round %d enemies spawned!\n", round_counter);
    }

    // =====================================================
    // 3. CHECK ENEMY STATUS
    // =====================================================
    if (enemies_spawned && !enemy_defeated) {
      
        bool all_defeated = true;
    
        for (Elements* ele : elements) {
            switch (ele->label) {
                case Enemy_L: {
                    Enemy* e = dynamic_cast<Enemy*>(ele);
                    if (e && e->hp > 0) {
                        all_defeated = false;
                        break;
                    }
                    break;
                }
                case Enemy2_L: {
                    Enemy2* e2 = dynamic_cast<Enemy2*>(ele);
                    if (e2 && e2->hp > 0) {
                        all_defeated = false;
                        break;
                    }
                    break;
                }
                case Enemy3_L: {
                    Enemy3* e3 = dynamic_cast<Enemy3*>(ele);
                    if (e3 && e3->hp > 0) {
                        all_defeated = false;
                        break;
                    }
                    break;
                }
            }
            if (!all_defeated) break; // 有活的敵人 → 停止搜尋
        }

        if (all_defeated) {
            printf("DEFEATED, %d\n", enemy_count);
            enemy_defeated = true;
            round_locked = true;
            round_advancing = true;
            advance_timer = 0;
            printf("[GameScene] Round %d cleared!\n", round_counter);
        }
    }

    // =====================================================
    // 4. MANUAL TEST (T key)
    // =====================================================
    if (key_state[ALLEGRO_KEY_T] && !round_advancing) {
        printf("[GameScene] Manual skip: round %d cleared.\n", round_counter);
        enemy_defeated = true;
        round_advancing = true;
        advance_timer = 0;
    }

    // =====================================================
    // 5. ROUND ADVANCING (BGM fade-out)
    // =====================================================
    if (round_advancing) {
        // BGM fade out
        if (round_bgm_instance) {
            float volume = 1.0f - (advance_timer / (float)(FPS * 2));
            if (volume < 0.0f) volume = 0.0f;
            al_set_sample_instance_gain(round_bgm_instance, volume);
        }

        advance_timer++;

        if (advance_timer > FPS * 2) {

            if (round_bgm_instance) {
                al_stop_sample_instance(round_bgm_instance);
                al_destroy_sample_instance(round_bgm_instance);
                round_bgm_instance = NULL;
            }

            // Move to next round
            round_counter++;
            round_advancing = false;

            if (round_counter > 3) {
                // === ALL ROUNDS FINISHED ===
                printf("[GameScene] All rounds completed!\n");
                scene_end = true;
                create_scene(VictoryScene_L);
                return;
            }

            // reset for next round
            enemies_spawned = false;
            round_triggered = true;
            round_timer = 0;
            round_locked = false;

            // Play new BGM
            round_bgm_instance = al_create_sample_instance(round_bgm[round_counter - 1]);
            
            al_set_sample_instance_playmode(round_bgm_instance, ALLEGRO_PLAYMODE_LOOP);
            al_attach_sample_instance_to_mixer(round_bgm_instance, al_get_default_mixer());
            al_set_sample_instance_gain(round_bgm_instance, 1.0f);
            al_play_sample_instance(round_bgm_instance);

            // Play sound cue
            al_play_sample(round_sounds[round_counter - 1], 
                1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

            printf("[GameScene] >>> ENTER ROUND %d <<<\n", round_counter);
        }

        return; // fade-out 時不做以下邏輯
    }

    // =====================================================
    // 6. SCENE SWITCH KEYS
    // =====================================================
    if (key_state[ALLEGRO_KEY_Q]) {
        scene_end = true;
        if (round_bgm_instance) {
            al_stop_sample_instance(round_bgm_instance);
            al_destroy_sample_instance(round_bgm_instance);
            round_bgm_instance = NULL;
        }
        create_scene(VictoryScene_L);
    }
    if (key_state[ALLEGRO_KEY_E]) {
        scene_end = true;
        //for(int i = 0; i < 3; i++){
            if(round_bgm_instance){
                al_stop_sample_instance(round_bgm_instance);
                al_destroy_sample_instance(round_bgm_instance);
                round_bgm_instance = NULL;
                printf("[GameScene] BGM stopped and destroyed.\n");
            }
       // }
        //al_destroy_sample(song);
        create_scene(DeathScene_L);
    }
    if (key_state[ALLEGRO_KEY_Z]) {
        if(round_bgm_instance){
                al_stop_sample_instance(round_bgm_instance);
                al_destroy_sample_instance(round_bgm_instance);
                round_bgm_instance = NULL;
                printf("[GameScene] BGM stopped and destroyed.\n");
        }
        scene_end = true;
        create_scene(CreditScene_L);
    }
}


void GameScene::Draw() {

    //printf("GameScene::Draw elements size = %d\n", (int)elements.size());

    // === DRAW BACKGROUND ===
    al_clear_to_color(al_map_rgb(0, 0, 0));
    int bg_index = round_counter - 1;
    if (bg_index < 0) bg_index = 0;
    if (bg_index > 2) bg_index = 2; // safety

    if (background[bg_index])
        al_draw_bitmap(background[bg_index], 0, 0, 0);

    // === DRAW FLOOR ===
    for (Elements* e : elements) {
        if (e->label == Floor_L && !e->dele)
            e->Draw(); // C++ virtual Draw()
    }

    // === ROUND INDICATOR ===
    if (round_triggered && round_counter <= 3) {
        ALLEGRO_BITMAP* img = round_images[round_counter - 1];
        if (img) {
            int img_w = al_get_bitmap_width(img);
            al_draw_bitmap(img, WIDTH / 2 - img_w / 2, 100, 0);
        }
    }

    // === DRAW ALL ELEMENTS ===
    for (Elements* ele : elements) {

        if (ele->dele) continue; // ignore deleted ones

        // --------------------------
        // CHARACTER 2 (Player 2)
        // --------------------------
        if (ele->label == Character2_L) {

            Character2* ch2 = dynamic_cast<Character2*>(ele);
            if (!ch2) continue;

            char buf[64];

            // Draw sprite
            if (ch2->blood > 0)
                ele->Draw();

            // name
            if(ch2->blood > 0){
                sprintf(buf, "Character 2");
                al_draw_text(font, al_map_rgb(255, 0, 0), ch2->x, ch2->y - 20, 0, buf);
            }

            // HP
            sprintf(buf, "Character2 Blood: %d", ch2->blood);
            al_draw_text(font, al_map_rgb(255, 0, 0), 10, 70, 0, buf);

            // cooldown display J K L
            if (ch2->cool_J / FPS == 0)
                al_draw_text(font, al_map_rgb(255, 0, 0), 820, 70, 0, "J");
            else {
                sprintf(buf, "%d", ch2->cool_J / FPS);
                al_draw_text(font, al_map_rgb(200, 200, 200), 820, 70, 0, buf);
            }

            if (ch2->cool_K / FPS == 0)
                al_draw_text(font, al_map_rgb(255, 0, 0), 820, 70, 0, "  K");
            else {
                sprintf(buf, "  %d", ch2->cool_K / FPS);
                al_draw_text(font, al_map_rgb(200, 200, 200), 820, 70, 0, buf);
            }

            if (ch2->cool_L / FPS == 0)
                al_draw_text(font, al_map_rgb(255, 0, 0), 820, 70, 0, "    L");
            else {
                sprintf(buf, "    %d", ch2->cool_L / FPS);
                al_draw_text(font, al_map_rgb(200, 200, 200), 820, 70, 0, buf);
            }

            // blood bar
            int maxblood = 100 + (ch2->level * ch2->add_blood);
            float percent = (float)ch2->blood / maxblood;
            float barLength = percent * barlong;

            // background
            al_draw_filled_rectangle(298, 68, 812, 93, al_map_rgb(0, 0, 0)); // border
            al_draw_filled_rectangle(300, 70, 810, 90, al_map_rgb(150, 150, 150)); // base

            if (ch2->blood > 0)
                al_draw_filled_rectangle(300, 70, 300 + barLength, 90, al_map_rgb(255, 0, 0));

            // level progress
            int maxpoints = (ch2->level + 1) * ch2->add_blood;
            sprintf(buf, "Level:%d points: %d/%d", ch2->level, ch2->levelup_points, maxpoints);
            al_draw_text(font, al_map_rgb(0,255,0), 10, 100, 0, buf);

            percent = (float)ch2->levelup_points / maxpoints;
            barLength = percent * barlong;

            al_draw_filled_rectangle(298, 98, 812, 123, al_map_rgb(0,0,0));
            al_draw_filled_rectangle(300, 100, 810, 120, al_map_rgb(150,150,150));
            al_draw_filled_rectangle(300, 100, 300 + barLength, 120, al_map_rgb(0,255,0));
        }

        // --------------------------
        // CHARACTER 1 (Player 1)
        // --------------------------
        else if (ele->label == Character_L) {

            Character* ch = dynamic_cast<Character*>(ele);
            if (!ch) continue;

            char buf[64];

            if (ch->blood > 0)
                ele->Draw();
            
            if (ch->blood > 0){
                sprintf(buf, "Character 1");
                al_draw_text(font, al_map_rgb(255, 0, 0), ch->x, ch->y - 20, 0, buf);
            }
            
            // HP
            sprintf(buf, "Character1 Blood: %d", ch->blood);
            al_draw_text(font, al_map_rgb(255, 0, 0), 10, 10, 0, buf);

            // cooldown X C V
            if (ch->cool_X / FPS == 0)
                al_draw_text(font, al_map_rgb(255, 0, 0), 820, 10, 0, "X");
            else {
                sprintf(buf, "%d", ch->cool_X / FPS);
                al_draw_text(font, al_map_rgb(200, 200, 200), 820, 10, 0, buf);
            }

            if (ch->cool_C / FPS == 0)
                al_draw_text(font, al_map_rgb(255, 0, 0), 820, 10, 0, "  C");
            else {
                sprintf(buf, "  %d", ch->cool_C / FPS);
                al_draw_text(font, al_map_rgb(200, 200, 200), 820, 10, 0, buf);
            }

            if (ch->cool_V / FPS == 0)
                al_draw_text(font, al_map_rgb(255, 0, 0), 820, 10, 0, "    V");
            else {
                sprintf(buf, "    %d", ch->cool_V / FPS);
                al_draw_text(font, al_map_rgb(200, 200, 200), 820, 10, 0, buf);
            }

            // blood bar
            int maxblood = 100 + (ch->level * ch->add_blood);
            float percent = (float)ch->blood / maxblood;
            float barLength = percent * barlong;

            al_draw_filled_rectangle(298, 8, 812, 33, al_map_rgb(0,0,0));
            al_draw_filled_rectangle(300, 10, 810, 30, al_map_rgb(150,150,150));
            if (ch->blood > 0)
                al_draw_filled_rectangle(300, 10, 300 + barLength, 30, al_map_rgb(255, 0, 0));

            int maxpoints = (ch->level + 1) * ch->add_blood;
            sprintf(buf, "Level:%d points: %d/%d", ch->level, ch->levelup_points, maxpoints);
            al_draw_text(font, al_map_rgb(0,255,0), 10, 40, 0, buf);

            percent = (float)ch->levelup_points / maxpoints;
            barLength = percent * barlong;

            al_draw_filled_rectangle(298, 38, 812, 63, al_map_rgb(0,0,0));
            al_draw_filled_rectangle(300, 40, 810, 60, al_map_rgb(150,150,150));
            al_draw_filled_rectangle(300, 40, 300 + barLength, 60, al_map_rgb(0,255,0));
        }

        // --------------------------
        // OTHER ELEMENTS
        // --------------------------
        else {
            ele->Draw();     // 直接呼叫 virtual Draw()
        }
    }
}

GameScene::~GameScene() {
    for (int i = 0; i < 3; i++) {
        if (round_images[i])
            al_destroy_bitmap(round_images[i]);
        if (round_sounds[i])
            al_destroy_sample(round_sounds[i]);
        if (round_bgm[i])
            al_destroy_sample(round_bgm[i]);
        if (background[i]) 
            al_destroy_bitmap(background[i]);
    }

    if (round_bgm_instance) {
        al_stop_sample_instance(round_bgm_instance);
        al_destroy_sample_instance(round_bgm_instance);
    }

    if (box_image)
        al_destroy_bitmap(box_image);

    if (gentong_image)
        al_destroy_bitmap(gentong_image);

    printf("[GameScene] Destroyed.\n");
}







