#ifndef STARTSCENE_H
#define STARTSCENE_H

#include "scene.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

typedef struct _StartScene
{
    ALLEGRO_BITMAP *background;
    ALLEGRO_SAMPLE *music;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;
    ALLEGRO_FONT *font;
    int timer;
} StartScene;

Scene *New_StartScene(int label);
void startScene_update(Scene *self);
void startScene_draw(Scene *self);
void startScene_destroy(Scene *self);

#endif