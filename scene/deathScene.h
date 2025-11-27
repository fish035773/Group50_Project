#ifndef DEATHSCENE_H_INCLUDED
#define DEATHSCENE_H_INCLUDED

#include "scene.h"
#include "sceneManager.h"
#include "../global.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>


typedef struct _DeathScene {
    ALLEGRO_BITMAP *background;
    ALLEGRO_SAMPLE *music;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;
    ALLEGRO_FONT *font;

    // ADD THIS:
    ALLEGRO_BITMAP *game_over_img;

    int timer;
} DeathScene;


Scene *New_DeathScene(int label);
void deathScene_update(Scene *self);
void deathScene_draw(Scene *self);
void deathScene_destroy(Scene *self);

#endif