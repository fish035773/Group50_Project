#include "deathScene.h"

Scene *New_DeathScene(int label)
{
    DeathScene *pDerivedObj = (DeathScene *)malloc(sizeof(DeathScene));
    Scene *pObj = New_Scene(label);

    printf("[DeathScene] Creating Death Scene...\n");

    // Load background
    pDerivedObj->background = al_load_bitmap("assets/image/game_over_bg.png");
    if (pDerivedObj->background)
        printf("[DeathScene] Background loaded OK.\n");
    else
        printf("WARNING: DeathScene background NOT FOUND!\n");

    // Load music
    pDerivedObj->music = al_load_sample("assets/sound/game_over.mp3");
    if (pDerivedObj->music) {
        printf("[DeathScene] Game Over music loaded OK.\n");
        pDerivedObj->sample_instance = al_create_sample_instance(pDerivedObj->music);
        if (pDerivedObj->sample_instance) {
            al_set_sample_instance_playmode(pDerivedObj->sample_instance, ALLEGRO_PLAYMODE_ONCE);
            al_attach_sample_instance_to_mixer(pDerivedObj->sample_instance, al_get_default_mixer());
            al_play_sample_instance(pDerivedObj->sample_instance);
            printf("[DeathScene] Music instance playing.\n");
        } else {
            printf("WARNING: Failed to create sample instance!\n");
        }
    } else {
        printf("WARNING: DeathScene music NOT FOUND!\n");
        pDerivedObj->sample_instance = NULL;
    }

    // Load font
    pDerivedObj->font = al_load_ttf_font("assets/font/pirulen.ttf", 48, 0);
    if (pDerivedObj->font)
        printf("[DeathScene] Font loaded OK.\n");
    else
        printf("WARNING: DeathScene font NOT FOUND!\n");

    // Load GAME OVER image
    pDerivedObj->game_over_img = al_load_bitmap("assets/image/game_over.png");
    if (pDerivedObj->game_over_img)
        printf("[DeathScene] Game Over image loaded OK.\n");
    else
        printf("WARNING: Game Over image NOT FOUND!\n");

    pDerivedObj->timer = 0;

    // Set scene functions
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = deathScene_update;
    pObj->Draw = deathScene_draw;
    pObj->Destroy = deathScene_destroy;

    printf("[DeathScene] Scene created successfully!\n");

    return pObj;
}

void deathScene_update(Scene *self)
{
    DeathScene *obj = (DeathScene *)(self->pDerivedObj);
    obj->timer++;

    // Auto back to Menu after 5 sec
    if (obj->timer > FPS * 5)
    {
        self->scene_end = true;
        window = Menu_L;
        printf("[DeathScene] Returning to Menu...\n");
    }
}

void deathScene_draw(Scene *self)
{
    DeathScene *obj = (DeathScene *)(self->pDerivedObj);

    // Draw background
    if (obj->background) {
        al_draw_bitmap(obj->background, 0, 0, 0);
    } else {
        al_clear_to_color(al_map_rgb(0, 0, 0)); // fallback
    }

    // Draw GAME OVER image CENTERED dynamically
    if (obj->game_over_img) {
        int img_w = al_get_bitmap_width(obj->game_over_img);
        int img_h = al_get_bitmap_height(obj->game_over_img);

        al_draw_bitmap(obj->game_over_img,
                       (WIDTH - img_w) / 2,
                       (HEIGHT - img_h) / 2,
                       0);
    }
}

void deathScene_destroy(Scene *self)
{
    DeathScene *obj = (DeathScene *)(self->pDerivedObj);

    printf("[DeathScene] Destroying scene...\n");

    if (obj->background) al_destroy_bitmap(obj->background);
    if (obj->music) al_destroy_sample(obj->music);
    if (obj->sample_instance) al_destroy_sample_instance(obj->sample_instance);
    if (obj->font) al_destroy_font(obj->font);
    if (obj->game_over_img) al_destroy_bitmap(obj->game_over_img);
    free(obj);
    free(self);

    printf("[DeathScene] Scene destroyed.\n");
}