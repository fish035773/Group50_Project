#include "deathScene.h"
#include "sceneManager.h"
#include "../global.h"
#include <cstdio>

DeathScene::DeathScene(int label)
    : Scene(label),
      background(nullptr),
      game_over_img(nullptr),
      music(nullptr),
      sample_instance(nullptr),
      font(nullptr),
      timer(0)
{
    printf("[DeathScene] Creating Death Scene...\n");

    // Load background
    background = al_load_bitmap("assets/image/game_over_bg.png");
    if (background) printf("[DeathScene] Background loaded OK.\n");
    else printf("WARNING: DeathScene background NOT FOUND!\n");

    // Load GAME OVER image
    game_over_img = al_load_bitmap("assets/image/game_over.png");
    if (game_over_img) printf("[DeathScene] Game Over image loaded OK.\n");
    else printf("WARNING: Game Over image NOT FOUND!\n");

    // Load music
    music = al_load_sample("assets/sound/game_over.mp3");
    if (music) {
        printf("[DeathScene] Game Over music loaded OK.\n");
        sample_instance = al_create_sample_instance(music);
        if (sample_instance) {
            al_set_sample_instance_playmode(sample_instance, ALLEGRO_PLAYMODE_ONCE);
            al_attach_sample_instance_to_mixer(sample_instance, al_get_default_mixer());
            al_play_sample_instance(sample_instance);
            printf("[DeathScene] Music instance playing.\n");
        } else {
            printf("WARNING: Failed to create sample instance!\n");
        }
    } else {
        printf("WARNING: DeathScene music NOT FOUND!\n");
    }

    // Load font
    font = al_load_ttf_font("assets/font/pirulen.ttf", 48, 0);
    if (font) printf("[DeathScene] Font loaded OK.\n");
    else printf("WARNING: DeathScene font NOT FOUND!\n");

    printf("[DeathScene] Scene created successfully!\n");
}

DeathScene::~DeathScene() {
    Destroy();
}

void DeathScene::Update()
{
    timer++;

    // Auto back to menu after 5 seconds
    if (timer > FPS * 5) {
        printf("[DeathScene] Auto return to Menu.\n");
        create_scene(Menu_L);
        scene_end = true;
    }

    // ESC also returns
    if (key_state[ALLEGRO_KEY_ESCAPE]) {
        printf("[DeathScene] ESC return to Menu.\n");
        create_scene(Menu_L);
        scene_end = true;
    }
}

void DeathScene::Draw()
{
    // Draw background
    if (background)
        al_draw_bitmap(background, 0, 0, 0);
    else
        al_clear_to_color(al_map_rgb(0,0,0));  // fallback

    // Draw GAME OVER image centered
    if (game_over_img) {
        int img_w = al_get_bitmap_width(game_over_img);
        int img_h = al_get_bitmap_height(game_over_img);

        al_draw_bitmap(
            game_over_img,
            (WIDTH - img_w) / 2,
            (HEIGHT - img_h) / 2,
            0
        );
    }
    
    // Optional: draw text
    if (font) {
        al_draw_text(
            font,
            al_map_rgb(255, 0, 0),
            WIDTH / 2,
            HEIGHT * 0.75,
            ALLEGRO_ALIGN_CENTER,
            "You Died"
        );
    }
}

void DeathScene::Destroy()
{
    printf("[DeathScene] Destroying...\n");

    if (background) {
        al_destroy_bitmap(background);
        background = nullptr;
    }

    if (game_over_img) {
        al_destroy_bitmap(game_over_img);
        game_over_img = nullptr;
    }

    if (sample_instance) {
        al_destroy_sample_instance(sample_instance);
        sample_instance = nullptr;
    }

    if (music) {
        al_destroy_sample(music);
        music = nullptr;
    }

    if (font) {
        al_destroy_font(font);
        font = nullptr;
    }

    printf("[DeathScene] Destroyed.\n");
}
