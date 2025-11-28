#include "startScene.h"
#include "sceneManager.h"
#include "../global.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

Scene *New_StartScene(int label)
{
    StartScene *pDerivedObj = (StartScene *)malloc(sizeof(StartScene));
    Scene *pObj = New_Scene(label);

    printf("[StartScene] Creating Start Scene...\n");

    // Load background
    pDerivedObj->background = al_load_bitmap("assets/image/start_bg.png");  // ganti sesuai file kamu
    if (pDerivedObj->background)
        printf("[StartScene] Background loaded OK.\n");
    else
        printf("WARNING: StartScene background NOT FOUND!\n");

    // Load music
    pDerivedObj->music = al_load_sample("assets/sound/start_music.mp3");  // ganti sesuai file kamu
    if (pDerivedObj->music) {
        printf("[StartScene] Start music loaded OK.\n");
        pDerivedObj->sample_instance = al_create_sample_instance(pDerivedObj->music);
        if (pDerivedObj->sample_instance) {
            al_set_sample_instance_playmode(pDerivedObj->sample_instance, ALLEGRO_PLAYMODE_ONCE);
            al_attach_sample_instance_to_mixer(pDerivedObj->sample_instance, al_get_default_mixer());
            al_play_sample_instance(pDerivedObj->sample_instance);
            printf("[StartScene] Music instance playing.\n");
        } else {
            printf("WARNING: Failed to create sample instance!\n");
        }
    } else {
        printf("WARNING: StartScene music NOT FOUND!\n");
        pDerivedObj->sample_instance = NULL;
    }

    // Load font
    pDerivedObj->font = al_load_ttf_font("assets/font/pirulen.ttf", 28, 0);
    if (pDerivedObj->font)
        printf("[StartScene] Font loaded OK.\n");
    else
        printf("WARNING: StartScene font NOT FOUND!\n");

    // Timer setup
    pDerivedObj->timer = 0;

    // Set scene functions
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = startScene_update;
    pObj->Draw = startScene_draw;
    pObj->Destroy = startScene_destroy;

    printf("[StartScene] Scene created successfully!\n");

    return pObj;
}

void startScene_update(Scene *self)
{
    StartScene *obj = (StartScene *)(self->pDerivedObj);
    obj->timer++;

    // Manual TEST — tekan S untuk mulai GameScene
    if (key_state[ALLEGRO_KEY_S]) {
        self->scene_end = true;
        window = GameScene_L;
        printf("[StartScene] Switching to Game Scene!\n");
    }

    // Auto balik ke Menu setelah 10 sec
    if (obj->timer > FPS * 10)
    {
        self->scene_end = true;
        window = Menu_L;
        printf("[StartScene] Returning to Menu...\n");
    }
}

void startScene_draw(Scene *self)
{
    StartScene *obj = (StartScene *)(self->pDerivedObj);

    // Draw background
    if (obj->background) {
        al_draw_bitmap(obj->background, 0, 0, 0);
    } else {
        al_clear_to_color(al_map_rgb(0, 0, 0));  // fallback clear screen
    }

    // Draw "Press S to Start" text
    if (obj->font) {
        al_draw_text(obj->font, al_map_rgb(255, 255, 0), WIDTH / 2, HEIGHT / 2 + 100, ALLEGRO_ALIGN_CENTER, "Press [S] to Start Game");
    }
}

void startScene_destroy(Scene *self)
{
    StartScene *obj = (StartScene *)(self->pDerivedObj);

    printf("[StartScene] Destroying scene...\n");

    if (obj->background) al_destroy_bitmap(obj->background);
    if (obj->music) al_destroy_sample(obj->music);
    if (obj->sample_instance) al_destroy_sample_instance(obj->sample_instance);
    if (obj->font) al_destroy_font(obj->font);

    free(obj);
    free(self);

    printf("[StartScene] Scene destroyed.\n");
}