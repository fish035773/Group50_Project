#include "creditScene.h"
#include "../global.h"
#include <stdio.h>

#define CREDIT_FONT_SIZE 24
#define CREDIT_SCROLL_SPEED 1.0f

// --- Function prototypes ---
void creditScene_update(Scene *self);
void creditScene_draw(Scene *self);
void creditScene_destroy(Scene *self);

// --- credit text ---
const char *credit_text[] = {
    "Group 40 - Final Project",
    "",
    "Special Thanks to:",
    "Professor Chen, and TAs",
    "",
    "Developers:",
    "Character Design & Programming:",
    "郭宥均",
    "",
    "Art & UI Design:",
    "施發梅",
    "",
    "Enemy Art & Programming:",
    "陳慧琦",
    "",
    "Music & Sound Effects:",
    "From pixabay.com",
    "",
    "Thank you for playing!",
    "",
    "",
    "--- THE END ---",
    NULL
};

// --- NEW ---
Scene *New_CreditScene(int label)
{
    CreditScene *pDerivedObj = (CreditScene *)malloc(sizeof(CreditScene));
    memset(pDerivedObj, 0, sizeof(CreditScene));

    Scene *pObj = New_Scene(label);

    // Load background image
    pDerivedObj->background = al_load_bitmap("assets/image/credit_bg.png");
    if (pDerivedObj->background)
        printf("[CreditScene] Background loaded OK.\n");
    else
        printf("WARNING: CreditScene background NOT FOUND!\n");

    // Load music
    pDerivedObj->music = al_load_sample("assets/sound/credit_music.mp3");
    if (pDerivedObj->music)
    {
        pDerivedObj->music_instance = al_create_sample_instance(pDerivedObj->music);
        al_set_sample_instance_playmode(pDerivedObj->music_instance, ALLEGRO_PLAYMODE_LOOP);
        al_attach_sample_instance_to_mixer(pDerivedObj->music_instance, al_get_default_mixer());
        al_play_sample_instance(pDerivedObj->music_instance);
        printf("[CreditScene] Credit music loaded OK.\n");
    }
    else
    {
        printf("WARNING: Credit music NOT FOUND!\n");
    }

    // Load font
   pDerivedObj->font = al_load_ttf_font("assets/font/NotoSansTC-Regular.ttf", CREDIT_FONT_SIZE, 0);
    if (pDerivedObj->font)
    {
        printf("[CreditScene] Font loaded OK.\n");
    }
    else
    {
        printf("WARNING: Credit font NOT FOUND!\n");
    }

    // Init scroll
    pDerivedObj->scroll_pos = HEIGHT;
    pDerivedObj->scroll_speed = CREDIT_SCROLL_SPEED;

    // Init scene timer
    pDerivedObj->scene_timer = 0.0f;

    // Calculate total height
    int line_count = 0;
    while (credit_text[line_count] != NULL)
        line_count++;

    pDerivedObj->total_text_height = line_count * (CREDIT_FONT_SIZE + 10); // line height spacing
    printf("[CreditScene] Scene created successfully!\n");

    // Set scene functions
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = creditScene_update;
    pObj->Draw = creditScene_draw;
    pObj->Destroy = creditScene_destroy;

    return pObj;
}

// --- UPDATE ---
void creditScene_update(Scene *self)
{
    CreditScene *obj = (CreditScene *)(self->pDerivedObj);

    // Update scroll
    obj->scroll_pos -= obj->scroll_speed;

    // Update scene timer
    obj->scene_timer += 1.0f / FPS;

    if (obj->scroll_pos < -obj->total_text_height)
    {
        obj->scroll_pos = HEIGHT;
        printf("[CreditScene] Restarting scroll.\n");

        // Auto return ke menu setelah ~15 detik
        if (obj->scene_timer > 15.0f) {
            self->scene_end = true;
            window = Menu_L;
            printf("[CreditScene] Auto-return to Menu after credit.\n");
        }
    }

    // ESC manual tetap boleh
    if (key_state[ALLEGRO_KEY_ESCAPE])
    {
        self->scene_end = true;
        window = Menu_L;
        printf("[CreditScene] Returning to Menu...\n");
    }
}

// --- DRAW ---
void creditScene_draw(Scene *self)
{
    CreditScene *obj = (CreditScene *)(self->pDerivedObj);

    // Draw background
    if (obj->background)
        al_draw_bitmap(obj->background, 0, 0, 0);
    else
        al_clear_to_color(al_map_rgb(0, 0, 0)); // fallback bg

    // Draw text
    if (obj->font)
    {
        float y = obj->scroll_pos;

        int i = 0;
        while (credit_text[i] != NULL)
        {
            al_draw_text(
                obj->font,
                al_map_rgb(255, 255, 255),
                WIDTH / 2,
                y,
                ALLEGRO_ALIGN_CENTER,
                credit_text[i]
            );

            y += CREDIT_FONT_SIZE + 10; // spacing
            i++;
        }
    }
}

// --- DESTROY ---
void creditScene_destroy(Scene *self)
{
    CreditScene *obj = (CreditScene *)(self->pDerivedObj);

    // Destroy background
    if (obj->background)
        al_destroy_bitmap(obj->background);

    // Stop and destroy music
    if (obj->music_instance)
    {
        al_stop_sample_instance(obj->music_instance);
        al_destroy_sample_instance(obj->music_instance);
    }

    if (obj->music)
    {
        al_destroy_sample(obj->music);
    }

    // Destroy font
    if (obj->font)
    {
        al_destroy_font(obj->font);
    }

    free(obj);
    printf("[CreditScene] Scene destroyed.\n");
}