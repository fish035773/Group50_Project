#pragma once
#include "scene.h"
#include "sceneManager.h"  // buat Menu_L

// FONT
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

// AUDIO
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

// IMAGE (optional, kalau ada background image)
#include <allegro5/allegro_image.h>

typedef struct _CreditScene
{
    ALLEGRO_BITMAP *background;

    ALLEGRO_SAMPLE *music;
    ALLEGRO_SAMPLE_INSTANCE *music_instance;

    ALLEGRO_FONT *font;

    float scroll_pos;
    float scroll_speed;
    int total_text_height;

    float scene_timer; // TAMBAHAN BONUS

} CreditScene;


Scene *New_CreditScene(int label);