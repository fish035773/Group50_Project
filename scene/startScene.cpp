#include "startScene.h"
#include "sceneManager.h"
#include "../global.h"
#include <cstdio>

StartScene::StartScene(int label)
    : Scene(label),
      background(nullptr),
      music(nullptr),
      sample_instance(nullptr),
      font(nullptr),
      timer(0)
{
    printf("[Start] Creating Start Scene...\n");

    // Load background
    background = al_load_bitmap("assets/image/start_bg.png");
    if (background)
        printf("[Start] Background loaded OK.\n");
    else
        printf("WARNING: Start background NOT FOUND!\n");

    // Load music
    music = al_load_sample("assets/sound/start_music.mp3");
    if (music) {
        printf("[Start] Start music loaded OK.\n");
        sample_instance = al_create_sample_instance(music);
        if (sample_instance) {
            al_set_sample_instance_playmode(sample_instance, ALLEGRO_PLAYMODE_ONCE);
            al_attach_sample_instance_to_mixer(sample_instance, al_get_default_mixer());
            al_play_sample_instance(sample_instance);
            printf("[Start] Music playing.\n");
        } else {
            printf("WARNING: Failed to create sample instance!\n");
        }
    } else {
        printf("WARNING: Start music NOT FOUND!\n");
    }

    // Load font
    font = al_load_ttf_font("assets/font/pirulen.ttf", 28, 0);
    if (font)
        printf("[Start] Font loaded OK.\n");
    else
        printf("WARNING: Start font NOT FOUND!\n");

    timer = 0;

    printf("[Start] Scene created successfully!\n");
}

StartScene::~StartScene()
{
    Destroy();
}

void StartScene::Update()
{
    timer++;

    // PRESS S → ENTER GAME
    if (key_state[ALLEGRO_KEY_S]) {
        printf("[Start] S pressed — Switching to GameScene\n");
        create_scene(GameScene_L);
        scene_end = true;
        return;
    }

    // Auto return to Menu after 10 sec
    if (timer > FPS * 10) {
        printf("[Start] Auto-return to Menu\n");
        create_scene(Menu_L);
        scene_end = true;
    }
}

void StartScene::Draw()
{
    // Draw background
    if (background)
        al_draw_bitmap(background, 0, 0, 0);
    else
        al_clear_to_color(al_map_rgb(0,0,0));

    // Draw text
    if (font) {
        al_draw_text(
            font,
            al_map_rgb(255, 255, 0),
            WIDTH / 2,
            HEIGHT / 2 + 100,
            ALLEGRO_ALIGN_CENTER,
            "Press [S] to Start Game"
        );
    }
}

void StartScene::Destroy()
{
    printf("[Start] Destroying Start Scene...\n");

    if (background) {
        al_destroy_bitmap(background);
        background = nullptr;
    }

    if (music) {
        al_destroy_sample(music);
        music = nullptr;
    }

    if (sample_instance) {
        al_destroy_sample_instance(sample_instance);
        sample_instance = nullptr;
    }

    if (font) {
        al_destroy_font(font);
        font = nullptr;
    }

    printf("[Start] Scene destroyed.\n");
}
